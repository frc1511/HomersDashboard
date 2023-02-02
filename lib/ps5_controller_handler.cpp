#include <HomersDashboard/ps5_contoller_handler.h>
#include <Windows.h>

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
  
  if (reload_connections()) {
    has_init = true;
  }
}

void PS5ControllerHandler::process() {
  DS5W::DS5InputState in_state;

  if (valid_driver) {
    if (DS5W_SUCCESS(DS5W::getDeviceInputState(&driver_context, &in_state))) {

    }

    DS5W::DS5OutputState out_state;
    ZeroMemory(&out_state, sizeof(out_state));

    DS5W::setDeviceOutputState(&driver_context, &out_state);
  }

  if (valid_aux) {
    if (DS5W_SUCCESS(DS5W::getDeviceInputState(&aux_context, &in_state))) {

    }

    DS5W::DS5OutputState out_state;
    ZeroMemory(&out_state, sizeof(out_state));

    DS5W::setDeviceOutputState(&aux_context, &out_state);
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

  if (count < 2) {
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
}

void PS5ControllerHandler::set_controller_id(int driver, int aux) {
  driver_id = driver;
  aux_id = aux;
  reload_connections();
}

PS5ControllerHandler PS5ControllerHandler::instance;