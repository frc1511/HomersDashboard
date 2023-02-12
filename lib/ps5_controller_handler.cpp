#include <HomersDashboard/ps5_contoller_handler.h>
#define _WINSOCKAPI_
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

PS5ControllerHandler::PS5ControllerHandler() { }

PS5ControllerHandler::~PS5ControllerHandler() {
    if (valid_driver) {
        DS5W::freeDeviceContext(&driver_context);
    }
    if (valid_aux) {
        DS5W::freeDeviceContext(&aux_context);
    }
}

void PS5ControllerHandler::init() {
  if (has_init) {
    return;
  }
  
  reload_connections();

  WSADATA wsa_data;
  int res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (res != 0) {
    fmt::print("WSAStartup() failed with error: {}\n", res);
    return;
  }

  networking_thread = std::thread([this]() {
    this->thread_main();
  });

  has_init = true;
}

void PS5ControllerHandler::process() {
  if (valid_driver) {
    OutputState output;
    {
      std::lock_guard<std::mutex> lock(output_mutex);
      output = driver_output;
    }
    if (testing_driver) {
      output.rumbleLeft = 0xFF;
      output.rumbleRight = 0xFF;
    }

    handle_controller_output(driver_context, output);

    InputState _new_driver_input;
    handle_controller_input(driver_context, &_new_driver_input);

    std::lock_guard<std::mutex> lock(input_mutex);
    driver_input = _new_driver_input;
  }

  if (valid_aux) {
    OutputState output;
    {
      std::lock_guard<std::mutex> lock(output_mutex);
      output = aux_output;
    }
    if (testing_aux) {
      output.rumbleLeft = 0xFF;
      output.rumbleRight = 0xFF;
    }

    handle_controller_output(aux_context, output);

    InputState _new_aux_input;
    handle_controller_input(aux_context, &_new_aux_input);

    std::lock_guard<std::mutex> lock(input_mutex);
    aux_input = _new_aux_input;
  }

  {
    std::lock_guard<std::mutex> lock(input_mutex);
    new_input = true;
  }
}

bool PS5ControllerHandler::reload_connections() {
  DS5W::DeviceEnumInfo infos[16];

  unsigned int count;

  switch (DS5W::enumDevices(infos, 16, &count)) {
    case DS5W_OK:
    case DS5W_E_INSUFFICIENT_BUFFER:
      break;
    default:
      return false;
  }

  if (DS5W_FAILED(DS5W::initDeviceContext(&infos[driver_id], &driver_context))){
    valid_driver = false;
  }
  else {
    valid_driver = true;
  }

  if (DS5W_FAILED(DS5W::initDeviceContext(&infos[aux_id], &aux_context))){
    valid_aux = false;
  }
  else {
    valid_aux = true;
  }

  if (valid_aux && valid_driver) {
    return true;
  }
  return false;
}

void PS5ControllerHandler::set_controller_ids(int driver, int aux) {
  driver_id = driver;
  aux_id = aux;
  reload_connections();
}

void PS5ControllerHandler::thread_main() {
  using namespace std::chrono_literals;

SOCKET_CREATE:
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket == INVALID_SOCKET) {
    std::cout << "socket() failed." << std::endl;
    std::this_thread::sleep_for(1s);
    goto SOCKET_CREATE;
  }

  u_long non_blocking_mode = 1;
  if (ioctlsocket(client_socket, FIONBIO, &non_blocking_mode) == SOCKET_ERROR) {
    std::cout << "ioctlsocket() failed." << std::endl;
    closesocket(client_socket);
    goto SOCKET_CREATE;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5809); // TODO: Make port configurable.
  server_addr.sin_addr.s_addr = inet_addr("10.15.11.20"); // TODO: Make IP configurable.

SOCKET_CONNECT:
  if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
    int err = WSAGetLastError();
    if (err != WSAEWOULDBLOCK) {
      std::cout << "connect() failed " << WSAGetLastError() << std::endl;
      closesocket(client_socket);
      goto SOCKET_CREATE;
    }
    
    // Wait for the connection to succeed.
    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(client_socket, &write_set);
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int select_res = select(0, NULL, &write_set, NULL, &timeout);
    if (select_res == 0) {
      std::cout << "select() timed out." << std::endl;
      closesocket(client_socket);
      goto SOCKET_CREATE;
    }
    else if (select_res == SOCKET_ERROR) {
      std::cout << "select() failed: " << WSAGetLastError() << std::endl;
      closesocket(client_socket);
      goto SOCKET_CREATE;
    }
  }

  std::cout << "Connected to server :D" << std::endl;

  char input_buf[sizeof(InputMessage)];
  char output_buf[256];

  // Clear socket.
  recv(client_socket, output_buf, sizeof(output_buf), 0);
  ZeroMemory(output_buf, sizeof(output_buf));

  std::size_t bytes_received = 0;
  while (true) {

    // --- Recieve output ---

SOCKET_READ:
    int recv_res = recv(client_socket, output_buf + bytes_received, sizeof(output_buf) - bytes_received, 0);
    if (recv_res == SOCKET_ERROR) {
      int err = WSAGetLastError();
      if (err != WSAEWOULDBLOCK) {
        std::cout << "recv() failed: " << err << '\n';
        closesocket(client_socket);
        goto SOCKET_CREATE;
      }
      if (bytes_received > 0) {
        // You've got mail!!!

        fmt::print("Received {} bytes\n", recv_res);
        {
          OutputMessage msg;
          std::memcpy(&msg, output_buf, sizeof(OutputMessage));

          // Handle recieving output.
          std::lock_guard<std::mutex> lock(output_mutex);
          driver_output = msg.driverOutputState;
          aux_output = msg.auxOutputState;
        }

        ZeroMemory(output_buf, sizeof(output_buf));
        bytes_received = 0;
      }
    }
    else {
      bytes_received += recv_res;
      goto SOCKET_READ;
    }

    // --- Send input ---

    bool _new_input = false;

    {
      std::lock_guard<std::mutex> lock(input_mutex);
      _new_input = new_input;
    }

    // Send input.
    if (_new_input) {
      InputMessage msg;
      {
        std::lock_guard<std::mutex> lock(input_mutex);
        msg.driverInputState = driver_input;
        msg.auxInputState = aux_input;
        new_input = false;
      }
      std::memcpy(input_buf, &msg, sizeof(InputMessage));

      if (send(client_socket, input_buf, sizeof(InputMessage), 0) == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
          std::cout << "send() failed: " << err << '\n';
          closesocket(client_socket);
          goto SOCKET_CREATE;
        }
      }
    }
  }
}

void PS5ControllerHandler::handle_controller_input(DS5W::DeviceContext& context, InputState* input) {
  DS5W::DS5InputState input_state;

  // Retrieve data
  if (DS5W_FAILED(DS5W::getDeviceInputState(&context, &input_state))) {
    return;
  }

  input->axisLeftX = input_state.leftStick.x;
  input->axisLeftY = input_state.leftStick.y;

  input->axisRightX = input_state.rightStick.x;
  input->axisRightY = input_state.rightStick.y;

  input->axisLeftTrigger = input_state.leftTrigger;
  input->axisRightTrigger = input_state.rightTrigger;

  input->buttonsAndDPad = input_state.buttonsAndDpad;
  input->buttonsA = input_state.buttonsA;
  input->buttonsB = input_state.buttonsB;

  auto accel = input_state.accelerometer;
  input->accelX = accel.x;
  input->accelY = accel.y;
  input->accelZ = accel.z;

  auto gyro = input_state.gyroscope;
  input->gyroX = gyro.x;
  input->gyroY = gyro.y;
  input->gyroZ = gyro.z;
}

void PS5ControllerHandler::handle_controller_output(DS5W::DeviceContext& context, const OutputState& output) {
  DS5W::DS5OutputState out_state;
  ZeroMemory(&out_state, sizeof(out_state));

  // Rumble

  out_state.leftRumble = output.rumbleLeft;
  out_state.rightRumble = output.rumbleRight;

  // Triggers

  switch (output.leftTriggerEffect) {
    case 0:
    default:
      out_state.leftTriggerEffect.effectType = DS5W::TriggerEffectType::NoResitance;
      break;
    case 1:
      out_state.leftTriggerEffect.effectType = DS5W::TriggerEffectType::SectionResitance;
      out_state.leftTriggerEffect.Section.startPosition = output.leftTriggerStartPosition;
      out_state.leftTriggerEffect.Section.endPosition = output.leftTriggerEndPosition;
      break;
    case 2:
      out_state.leftTriggerEffect.effectType = DS5W::TriggerEffectType::ContinuousResitance;
      out_state.leftTriggerEffect.Continuous.force = output.leftTriggerForce;
      out_state.leftTriggerEffect.Continuous.startPosition = output.leftTriggerStartPosition;
      break;
  }

  switch (output.rightTriggerEffect) {
    case 0:
    default:
      out_state.rightTriggerEffect.effectType = DS5W::TriggerEffectType::NoResitance;
      break;
    case 1:
      out_state.rightTriggerEffect.effectType = DS5W::TriggerEffectType::SectionResitance;
      out_state.rightTriggerEffect.Section.startPosition = output.rightTriggerStartPosition;
      out_state.rightTriggerEffect.Section.endPosition = output.rightTriggerEndPosition;
      break;
    case 2:
      out_state.rightTriggerEffect.effectType = DS5W::TriggerEffectType::ContinuousResitance;
      out_state.rightTriggerEffect.Continuous.force = output.rightTriggerForce;
      out_state.rightTriggerEffect.Continuous.startPosition = output.rightTriggerStartPosition;
      break;
  }

  // LEDs

  // Lightbar
  out_state.lightbar = DS5W::Color { (unsigned char)output.lightbarR, (unsigned char)output.lightbarG, (unsigned char)output.lightbarB };

  // Microphone
  out_state.microphoneLed = static_cast<DS5W::MicLed>(output.micLed);

  // Player LEDs
  out_state.playerLeds.playerLedFade = output.playerLedFade;
  out_state.playerLeds.bitmask = output.playerLedBitmask;
  out_state.playerLeds.brightness = DS5W::LedBrightness::HIGH;

  DS5W::setDeviceOutputState(&context, &out_state);
}

PS5ControllerHandler PS5ControllerHandler::instance;