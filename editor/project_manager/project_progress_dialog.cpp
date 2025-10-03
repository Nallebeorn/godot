/**************************************************************************/
/*  project_progress_dialog.cpp                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "project_progress_dialog.h"

#include "editor/themes/editor_scale.h"
#include "main/main.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/progress_bar.h"

ProjectProgressDialog::ProjectProgressDialog() {
	set_exclusive(true);
	set_flag(FLAG_POPUP, false);

	VBoxContainer *vb = memnew(VBoxContainer);
	add_child(vb);
	title = memnew(Label);
	title->set_theme_type_variation("HeaderSmall");
	vb->add_child(title);
	progress_bar = memnew(ProgressBar);
	vb->add_child(progress_bar);
	current_file = memnew(Label);
	current_file->set_text_overrun_behavior(TextServer::OVERRUN_TRIM_ELLIPSIS);
	vb->add_child(current_file);
}

void ProjectProgressDialog::set_current_file(String p_file) {
	current_file->set_text(p_file);
}

void ProjectProgressDialog::set_total_files(size_t p_total) {
	progress_bar->set_indeterminate(p_total == 0);
	progress_bar->set_max(p_total);
}

void ProjectProgressDialog::step() {
	if (progress_bar->get_max() > 0) {
		progress_bar->set_value(++completed_files);
	}

	if (OS::get_singleton()->get_ticks_msec() - last_tick >= 100) {
		_update_ui();
	}
}

void ProjectProgressDialog::show_dialog(String p_title) {
	title->set_text(p_title);
	current_file->set_text("");
	set_total_files(0);
	completed_files = 0;
	progress_bar->set_value(0);

	reset_size();
	popup_centered(Vector2(450, 0) * EDSCALE);

	_update_ui();
}

void ProjectProgressDialog::_update_ui() {
	last_tick = OS::get_singleton()->get_ticks_msec();
	ERR_FAIL_COND(!is_inside_tree());
	DisplayServer::get_singleton()->process_events();
	Main::iteration();
}
