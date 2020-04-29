/*
  ==============================================================================

    PluginUtils.h
    Created: 29 Apr 2020 11:02:40am
    Author:  mwicat

  ==============================================================================
*/

#pragma once

inline void handleResult(Result result, String action)
{
    if (!result.wasOk())
    {
        AlertWindow::showMessageBox (
            AlertWindow::AlertIconType::WarningIcon,
            "Error",
            "Error occured while " + action + ": " + result.getErrorMessage());
    }
}
