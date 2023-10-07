#include <stdlib.h>
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
    DigitalFilter *filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (filter == NULL) {
        filter = filter_make_savgol(
            sqlite3_value_int(apArg[1]), 
            sqlite3_value_int(apArg[2]), 
            sqlite3_value_int(apArg[3]));
    }

    filter_evaluate(sqlite3_value_double(apArg[0]), filter);
    filter->window_size++;
}

static void sqldsp_savgol_final(sqlite3_context *ctx) {
    DigitalFilter *filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    sqlite3_result_double(ctx, filter_current_value(filter));
    filter_free(filter);
}

static void sqldsp_savgol_inverse(
    sqlite3_context *ctx,
    int nArg,
    sqlite3_value *apArg[]
) {
    DigitalFilter *filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    if (filter->window_size > filter->n_feedforward) {
        filter->window_size--;
    }
    else {
        sqlite3_result_error(
            ctx, 
            "Error: the window size must never be less than the filter length", 
            SQLITE_MISUSE
        );
    }
}

static void sqldsp_savgol_value(sqlite3_context *ctx) {
    DigitalFilter *filter = sqlite3_aggregate_context(ctx, sizeof(DigitalFilter *));
    sqlite3_result_double(ctx, filter_current_value(filter));
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