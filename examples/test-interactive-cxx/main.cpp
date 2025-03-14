/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   main.cpp
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser test
 *
 *
 */
#include <iostream>
#include "scpi/scpi.h"
#include "../common-cxx/scpi-def.h"

size_t SCPI_Write(const scpi_t * context, const char * data, const size_t len) {
    (void) context;
    std::cout.write(data, static_cast<std::streamsize>(len));
    return len;
}

scpi_result_t SCPI_Flush(const scpi_t * context) {
    (void) context;
    std::cout << std::flush;
    return SCPI_RES_OK;
}

int SCPI_Error(const scpi_t * context, const int_fast16_t err) {
    (void) context;
    std::cerr << "**ERROR: " << err << ", \"" << SCPI_ErrorTranslate(static_cast<int16_t>(err)) << "\"\n";
    return 0;
}

scpi_result_t SCPI_Control(const scpi_t * context, const scpi_ctrl_name_t ctrl, const scpi_reg_val_t val) {
    (void) context;

    if (SCPI_CTRL_SRQ == ctrl) {
        std::cerr << "**SRQ: 0x" << std::hex << val << "(" << std::dec << val << ")\n";
    }
    else {
        std::cerr << "**CTRL: " << std::hex << ctrl << ": 0x" << std::hex << val << "(" << std::dec << val << ")\n";
    }
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(const scpi_t * context) {
    (void) context;

    std::cerr << "**Reset\n";
    return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemCommTcpipControlQ(const scpi_t * context) {
    (void) context;

    return SCPI_RES_ERR;
}

/*
 *
 */
[[noreturn]] int main(const int argc, char ** argv) {
    (void) argc;
    (void) argv;

    SCPI_Init(&scpi_context,
              scpi_commands,
              &scpi_interface,
              scpi_units_def,
              SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
              scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
              scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);

    std::cerr << "SCPI Interactive demo\n";

    while (true) {
        char ch = static_cast<char>(std::cin.get());
        SCPI_Input(&scpi_context, &ch, 1);
    }
}
