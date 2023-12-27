#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_NEON_POW
RGB_MATRIX_EFFECT(BAND_SPIRAL_NEON_POW)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef RGB (*neon_spiral_runner_pow_args)(RGB rgb1, RGB rgb2, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);

bool neon_spiral_pow_runner_2colors(effect_params_t* params, neon_spiral_runner_pow_args neon_func) {
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

static RGB BAND_SPIRAL_NEON_POW_math(RGB rgb1, RGB rgb2, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    uint8_t  spiral_pattern        = dist - time - atan2_8(dy, dx);
    uint16_t spiral_pattern_pow    = pow(spiral_pattern, 1.5) / 16;
    uint8_t transition_smoothness = 25;
    uint8_t  smoothness_reversed   = 254 - transition_smoothness;
    uint16_t smoothing_spiral;
    if (spiral_pattern_pow > smoothness_reversed)
    {
        smoothing_spiral = spiral_pattern_pow - smoothness_reversed;
    }
    else
    {
        smoothing_spiral = 0;
    }
    smoothing_spiral         = smoothing_spiral * 254 / transition_smoothness;
    fract16 corrected_spiral;
    if (spiral_pattern_pow > smoothing_spiral)
    {
        corrected_spiral = spiral_pattern_pow - smoothing_spiral;
    }
    else
    {
        corrected_spiral = 0;
    }
    fract8  alpha            = lerp16by16((corrected_spiral * (254 + transition_smoothness)) / 255, corrected_spiral, smoothing_spiral);
    RGB     out;
    out.r = lerp8by8(rgb1.r, rgb2.r, alpha);
    out.g = lerp8by8(rgb1.g, rgb2.g, alpha);
    out.b = lerp8by8(rgb1.b, rgb2.b, alpha);
    return out;
}

bool BAND_SPIRAL_NEON_POW(effect_params_t* params) {
    return neon_spiral_pow_runner_2colors(params, &BAND_SPIRAL_NEON_POW_math);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_BAND_SPIRAL_NEON_POW
