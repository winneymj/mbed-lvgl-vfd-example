/* Example of mbed-lvgl with Noritake Itron VFD
 * Copyright (c) 2019 George "AGlass0fMilk" Beckstein
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NoritakeLVGL.h"
#include "UARTInterface.h"

#include "lv_disp.h"
#include "lv_label.h"
#include "lv_theme.h"
#include "lv_theme_mono.h"

#include "platform/mbed_wait_api.h"

#include <LittlevGL.h>

#define USART3_TX	PB_10
#define USART3_RX	PB_11
#define USART3_CTS	PB_13
#define USART3_RTS	PB_14
#define VFD_RESET	PD_6
#define VFD_TREADY	PD_4


UARTInterface display_interface(USART3_TX, USART3_RX, 38400);
NoritakeLVGL vfd(display_interface, VFD_RESET);

// Debug output console
mbed::UARTSerial console(USBTX, USBRX, 9600);

int main(void) {

	// Set up flow control pins
	display_interface.set_flow_control(mbed::SerialBase::RTSCTS, USART3_RTS, USART3_CTS);

	printf("Initializing VFD Demo...\r\n");

	vfd.init();
	wait_ms(100);
	vfd.display_on();

	LittlevGL& lvgl = LittlevGL::get_instance();
	lvgl.init();
	lvgl.add_display_driver(vfd);
	lvgl.set_default_display(vfd);

	// Initialize the GUI

	// Set the default theme to monochromatic
	lv_theme_t* theme = lv_theme_mono_init(0, NULL);
	lv_theme_set_current(theme);


	/*Create label on the screen. By default it will inherit the style of the screen*/
	lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(title, "Hello World!");
	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 8);  /*Align to the top*/
	lv_obj_set_hidden(title, false);

	lvgl.start();

	while(true) {
		lvgl.update();
		wait_ms(10);
	}
}
