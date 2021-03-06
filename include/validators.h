/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2004-2013 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file validators.h
 * @brief Custom text control validator definitions.
 */

#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <wx/valtext.h>
#include <wx/grid.h>

/**
 * Class GRID_CELL_TEXT_EDITOR
 *
 * This class works around a bug in wxGrid where the first keystroke doesn't get sent through
 * the validator if the editor wasn't already open.
 */
class GRID_CELL_TEXT_EDITOR : public wxGridCellTextEditor
{
public:
    GRID_CELL_TEXT_EDITOR();

    virtual void SetValidator(const wxValidator& validator) override;
    virtual void StartingKey(wxKeyEvent& event) override;

protected:
    wxScopedPtr<wxValidator> m_validator;
};


/**
 * Class FILE_NAME_CHAR_VALIDATOR
 *
 * This class provides a custom wxValidator object for limiting the allowable characters when
 * defining footprint names.  Since the introduction of the PRETTY footprint library format,
 * footprint names cannot have any characters that would prevent file creation on any platform.
 * The characters \/:*?|"<> are illegal and filtered by the validator.
 */
class FILE_NAME_CHAR_VALIDATOR : public wxTextValidator
{
public:
    FILE_NAME_CHAR_VALIDATOR( wxString* aValue = NULL );
};


/**
 * Class FILE_NAME_WITH_PATH_CHAR_VALIDATOR
 *
 * This class provides a custom wxValidator object for limiting the allowable characters when
 * defining file names with path, for instance in schematic sheet file names.
 * The characters *?|"<> are illegal and filtered by the validator,
 * but /\: are valid (\ and : only on Windows.)
 */
class FILE_NAME_WITH_PATH_CHAR_VALIDATOR : public wxTextValidator
{
public:
    FILE_NAME_WITH_PATH_CHAR_VALIDATOR( wxString* aValue = NULL );
};


/**
 * Class ENV_VAR_NAME_VALIDATOR
 *
 * This class provides a custom wxValidator object for limiting the allowable characters
 * when defining an environment varaible name in a text edit control.  Only uppercase,
 * numbers, and underscore (_) characters are valid and the first character of the name
 * cannot start with a number.  This is according to IEEE Std 1003.1-2001.  Even though
 * most systems support other characters, these characters guarantee compatibility for
 * all shells.
 */
class ENV_VAR_NAME_VALIDATOR : public wxTextValidator
{
public:
    ENV_VAR_NAME_VALIDATOR( wxString* aValue = NULL );
    ENV_VAR_NAME_VALIDATOR( const ENV_VAR_NAME_VALIDATOR& val );

    virtual ~ENV_VAR_NAME_VALIDATOR();

    // Make a clone of this validator (or return NULL) - currently necessary
    // if you're passing a reference to a validator.
    virtual wxObject *Clone() const override
    {
        return new ENV_VAR_NAME_VALIDATOR( *this );
    }

    void OnChar( wxKeyEvent& event );

    void OnTextChanged( wxCommandEvent& event );
};

namespace KIUI
{
/**
 * Call a text validator's TransferToWindow method without firing
 * a text change event.
 *
 * This is useful when you want to keep a validator in sync with other data,
 * but the act of changing it should not trigger other updates. It is the
 * validator equivalent of ChangeValue() compared to SetValue().
 *
 * This function blocks all events, but the same technique can be used to
 * selectively block events.
 *
 * @param aValidator the validator to update the control of
 */
void ValidatorTransferToWindowWithoutEvents( wxValidator& aValidator );

} // namespace KIUI

#endif  // #ifndef VALIDATORS_H
