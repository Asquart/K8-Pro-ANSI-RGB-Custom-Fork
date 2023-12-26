#ifdef ENABLE_RGB_MATRIX_NEON_CHEVRON
RGB_MATRIX_EFFECT(NEON_CHEVRON)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef RGB (*neon_chevron_runner_args)(RGB rgb1, RGB rgb2, uint8_t i, uint8_t time);

bool neon_chevron_runner_2colors(effect_params_t* params, neon_chevron_runner_args neon_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 2);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        RGB     rgb1 = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
        RGB     rgb2 = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv2);
        RGB     rgb  = neon_func(rgb1, rgb2, i, time);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static RGB NEON_CHEVRON_math(RGB rgb1, RGB rgb2, uint8_t i, uint8_t time) {
    uint8_t wave_pattern = abs8(g_led_config.point[i].x - k_rgb_matrix_center.x);
    uint8_t time_func    = -g_led_config.point[i].y - time;
    uint8_t alpha        = lerp8by8(0, 254, abs8(abs8(wave_pattern + time_func) - 127));
    alpha                = alpha * 2;
    alpha                = alpha * alpha / 255;
    RGB    out;
    out.r = lerp8by8(rgb1.r, rgb2.r, alpha);
    out.g = lerp8by8(rgb1.g, rgb2.g, alpha);
    out.b = lerp8by8(rgb1.b, rgb2.b, alpha);
    return out;
}

bool NEON_CHEVRON(effect_params_t* params) {
    return neon_chevron_runner_2colors(params, &NEON_CHEVRON_math);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_BAND_SPIRAL_NEON


//static HSV RAINBOW_MOVING_CHEVRON_math(HSV hsv, uint8_t i, uint8_t time) {
//    hsv.h += abs8(g_led_config.point[i].y - k_rgb_matrix_center.y) + (g_led_config.point[i].x - time);
//    return hsv;
//}
