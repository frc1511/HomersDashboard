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
  auto sd = frc1511::NTHandler::get()->get_smart_dashboard();

  const auto get_controller_options = [&](int32_t& options, const std::string& key) {
    int32_t _options = static_cast<int32_t>(sd->GetNumber(key, 0.0));
    std::memcpy(&options, &_options, sizeof(options));
  };

  get_controller_options(driver_options.rumble, "thunderdashboard_driver_rumble");
  get_controller_options(driver_options.left_trigger, "thunderdashboard_driver_left_trigger");
  get_controller_options(driver_options.right_trigger, "thunderdashboard_driver_right_trigger");
  get_controller_options(driver_options.leds, "thunderdashboard_driver_leds");

  get_controller_options(aux_options.rumble, "thunderdashboard_aux_rumble");
  get_controller_options(aux_options.left_trigger, "thunderdashboard_aux_left_trigger");
  get_controller_options(aux_options.right_trigger, "thunderdashboard_aux_right_trigger");
  get_controller_options(aux_options.leds, "thunderdashboard_aux_leds");

  if (valid_driver) {
    handle_controller(driver_context, driver_options);
  }

  if (valid_aux) {
    handle_controller(aux_context, aux_options);
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

void PS5ControllerHandler::handle_controller(DS5W::DeviceContext& context, const Options& options) {
  DS5W::DS5OutputState out_state;
  ZeroMemory(&out_state, sizeof(out_state));

  // Rumble

  out_state.leftRumble = options.rumble.left;
  out_state.rightRumble = options.rumble.right;

  // Triggers

  auto handle_trigger = [](DS5W::TriggerEffect& effect, const PS5TriggerOptions& options) {
    switch (options.trigger_effect) {
      case 0:
      default:
        effect.effectType = DS5W::NoResitance;
        return;
      case 1:
        effect.effectType = DS5W::TriggerEffectType::SectionResitance;
        effect.Section.startPosition = options.start_position;
        effect.Section.endPosition = options.end_position;
        return;
      case 2: 
        effect.effectType = DS5W::TriggerEffectType::ContinuousResitance;
        effect.Continuous.force = options.force;
        effect.Continuous.startPosition = options.start_position;
        return;
    }
  }

  handle_trigger(out_state.leftTriggerEffect, options.left_trigger);
  handle_trigger(out_state.rightTriggerEffect, options.right_trigger);

  // LEDs

  // Lightbar
  out_state.lightbar = DS5W::Color { options.leds.lightbar_r, options.leds.lightbar_g, options.leds.lightbar_b };

  // Microphone
  out_state.microphoneLed = options.leds.microphone_led;

  // Player LEDs
  if (options.leds.player_bitmask) {
    out_state.playerLeds.playerLedFade = options.leds.player_fade;
    out_state.playerLeds.bitmask = options.leds.player_bitmask;
    out_state.playerLeds.brightness = DS5W::LedBrightness::HIGH;
  }

  DS5W::setDeviceOutputState(&context, &out_state);
}

PS5ControllerHandler PS5ControllerHandler::instance;