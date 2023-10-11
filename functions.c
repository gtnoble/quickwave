#include <stdlib.h>
#include <assert.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT3

#include "filter.h"

static void sqldsp_savgol_step(
    sqlite3_context *ctx, 
    int nArg, 
    sqlite3_value *apArg[]
);
static void sqldsp_savgol_final(sqlite3_context *ctx);
static void sqldsp_savgol_inverse(
    sqlite3_context *ctx,
    int nArg,
    sqlite3_value *apArg[]
);
static void sqldsp_savgol_value(sqlite3_context *ctx);


static void sqldsp_savgol_step(
    sqlite3_context *ctx, 
    int nArg, 
    sqlite3_value *apArg[]
){
    DigitalFilter **filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (*filter == NULL) {
        int status = filter_make_savgol(
            filter,
            sqlite3_value_int(apArg[1]), 
            sqlite3_value_int(apArg[2]), 
            sqlite3_value_int(apArg[3]));
        switch (status) {
            case ALLOCATION_FAILURE:
                sqlite3_result_error(ctx, "Error: Could not allocate savgol filter", -1);
                return;
            case IMPROPER_PARAMS:
                sqlite3_result_error(ctx, "Error: improper savgol parameters passed", -1);
                return;
            case FILTER_OK:
                break;
            default:
                sqlite3_result_error(ctx, "Error: an unknown error occured", -1);
                return;
        }
    }
    assert(*filter != NULL);

    filter_evaluate(sqlite3_value_double(apArg[0]), *filter);
    (*filter)->window_size++;
}

static void sqldsp_savgol_final(sqlite3_context *ctx) {
    DigitalFilter **filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (*filter == NULL) {
        sqlite3_result_error(ctx, "Error: savgol filter not allocated", -1);
        return;
    }
    sqlite3_result_double(ctx, filter_current_value(*filter));
    filter_free(*filter);
    *filter = NULL;
}

static void sqldsp_savgol_inverse(
    sqlite3_context *ctx,
    int nArg,
    sqlite3_value *apArg[]
) {
    DigitalFilter **filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (*filter == NULL) {
        sqlite3_result_error(ctx, "Error: savgol filter not allocated", -1);
        return;
    }
    if ((*filter)->window_size <= (*filter)->n_feedforward) {
        sqlite3_result_error(
            ctx, 
            "Error: the window size must never be less than the savgol filter length", 
            -1
        );
        return;
    }
    (*filter)->window_size--;
}

static void sqldsp_savgol_value(sqlite3_context *ctx) {
    DigitalFilter **filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (*filter == NULL) {
        sqlite3_result_error(ctx, "Error: savgol filter not allocated", -1);
        return;
    }
    sqlite3_result_double(ctx, filter_current_value(*filter));
}

int register_savgol(sqlite3 *db) {
    return sqlite3_create_window_function(
        db, 
        "savgol", 
        4, 
        SQLITE_UTF8, 
        NULL, 
        sqldsp_savgol_step,
        sqldsp_savgol_final,
        sqldsp_savgol_value,
        sqldsp_savgol_inverse,
        NULL
    );
}