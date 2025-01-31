/**
 * @file application_types.h
 * @author Travis Vroman (travis@kohiengine.com)
 * @brief This file contains types to be consumed by the application library.
 * @version 1.0
 * @date 2022-01-10
 * 
 * @copyright Kohi Game Engine is Copyright (c) Travis Vroman 2021-2022
 * 
 */

#pragma once

#include "core/engine.h"
#include "memory/linear_allocator.h"
#include "platform/platform.h"

struct render_packet;

typedef struct app_frame_data {
    // A darray of world geometries to be rendered this frame.
    geometry_render_data* world_geometries;
} app_frame_data;

/** @brief Represents the various stages of application lifecycle. */
typedef enum application_stage {
    /** @brief Application is in an uninitialized state. */
    APPLICATION_STAGE_UNINITIALIZED,
    /** @brief Application is currently booting up. */
    APPLICATION_STAGE_BOOTING,
    /** @brief Application completed boot process and is ready to be initialized. */
    APPLICATION_STAGE_BOOT_COMPLETE,
    /** @brief Application is currently initializing. */
    APPLICATION_STAGE_INITIALIZING,
    /** @brief Application initialization is complete. */
    APPLICATION_STAGE_INITIALIZED,
    /** @brief Application is currently running. */
    APPLICATION_STAGE_RUNNING,
    /** @brief Application is in the process of shutting down. */
    APPLICATION_STAGE_SHUTTING_DOWN
} application_stage;

/**
 * @brief Represents the basic application state in a application.
 * Called for creation by the application.
 */
typedef struct application {
    /** @brief The application configuration. */
    application_config app_config;

    /**
     * @brief Function pointer to the application's boot sequence. This should 
     * fill out the application config with the application's specific requirements.
     * @param app_inst A pointer to the application instance.
     * @returns True on success; otherwise false.
     */
    b8 (*boot)(struct application* app_inst);

    /** 
     * @brief Function pointer to application's initialize function. 
     * @param app_inst A pointer to the application instance.
     * @returns True on success; otherwise false.
     * */
    b8 (*initialize)(struct application* app_inst);

    /** 
     * @brief Function pointer to application's update function. 
     * @param app_inst A pointer to the application instance.
     * @param delta_time The time in seconds since the last frame.
     * @returns True on success; otherwise false.
     * */
    b8 (*update)(struct application* app_inst, f32 delta_time);

    /** 
     * @brief Function pointer to application's render function. 
     * @param app_inst A pointer to the application instance.
     * @param packet A pointer to the packet to be populated by the application.
     * @param delta_time The time in seconds since the last frame.
     * @returns True on success; otherwise false.
     * */
    b8 (*render)(struct application* app_inst, struct render_packet* packet, f32 delta_time);

    /** 
     * @brief Function pointer to handle resizes, if applicable. 
     * @param app_inst A pointer to the application instance.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     * */
    void (*on_resize)(struct application* app_inst, u32 width, u32 height);

    /**
     * @brief Shuts down the application, prompting release of resources.
     * @param app_inst A pointer to the application instance.
     */
    void (*shutdown)(struct application* app_inst);

    void (*lib_on_unload)(struct application* game_inst);

    void (*lib_on_load)(struct application* game_inst);

    /** @brief The application stage of execution. */
    application_stage stage;

    /** @brief application-specific state. Created and managed by the application. */
    void* state;

    /** @brief A block of memory to hold the engine state. Created and managed by the engine. */
    void* engine_state;

    /** 
     * @brief An allocator used for allocations needing to be made every frame. Contents are wiped
     * at the beginning of the frame.
     */
    linear_allocator frame_allocator;

    /** @brief Data which is built up, used and discarded every frame. */
    app_frame_data frame_data;

    // TODO: Move this to somewhere better...
    dynamic_library renderer_library;
    renderer_plugin render_plugin;

    dynamic_library game_library;
} application;
