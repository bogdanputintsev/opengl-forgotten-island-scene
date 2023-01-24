//----------------------------------------------------------------------------------------
/**
 * \file       Constants.h
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Contains all the constants in the project
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

static const int WINDOW_WIDTH = 1280;                         ///< Window width size
static const int WINDOW_HEIGHT = 720;                         ///< Window height size
static const char* WINDOW_TITLE = "Semester Work";            ///< Window title

static const char* vertexShaderPath = "vertexShader.vs";      ///< Path to a vertex shader
static const char* fragmentShaderPath = "fragmentShader.fs";  ///< Path to a fragment shader

static const int timerDelay = 33;                             ///< Timer event is called each 1/33 seconds

static const float mouseSensitivity = 0.3f;                   ///< Mouse sensitivity
static const float YAW_MIN = 0.0f;                            ///< Min value for yaw
static const float YAW_MAX = 360.0f;                          ///< Max value for yaw
static const float PITCH_MIN = -90.0f;                        ///< Min value for pitch
static const float PITCH_MAX = 90.0f;                         ///< Max value for pitch

static const int ESC_BUTTON = 27;                             ///< Char value for ESC key
static const char UP_KEY = 'q';                               ///< Char value for Q
static const char DOWN_KEY = 'e';                             ///< Char value for E

