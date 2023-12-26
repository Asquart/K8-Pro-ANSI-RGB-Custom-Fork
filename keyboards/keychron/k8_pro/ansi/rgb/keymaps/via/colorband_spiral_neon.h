#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_NEON
RGB_MATRIX_EFFECT(BAND_SPIRAL_NEON)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef RGB (*neon_runner_args)(RGB rgb1, RGB rgb2, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);

bool neon_spiral_runner_2colors(effect_params_t* params, neon_runner_args neon_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 2);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        int16_t dx   = g_led_config.point[i].x - (k_rgb_matrix_center.x - 70);
        int16_t dy   = g_led_config.point[i].y - k_rgb_matrix_center.y;
        uint8_t dist = sqrt16(dx * dx + dy * dy);
        RGB     rgb1 = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
        RGB     rgb2 = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv2);
        RGB     rgb  = neon_func(rgb1, rgb2, dx, dy, dist, time);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static RGB BAND_SPIRAL_NEON_math(RGB rgb1, RGB rgb2, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    fract8 alpha = dist - time - atan2_8(dy, dx);
    RGB   out;
    out.r = lerp8by8(rgb1.r, rgb2.r, alpha);
    out.g = lerp8by8(rgb1.g, rgb2.g, alpha);
    out.b = lerp8by8(rgb1.b, rgb2.b, alpha);
    return out;
}

bool BAND_SPIRAL_NEON(effect_params_t* params) {
    return neon_spiral_runner_2colors(params, &BAND_SPIRAL_NEON_math);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_BAND_SPIRAL_NEON
