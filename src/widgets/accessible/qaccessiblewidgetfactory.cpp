/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qaccessiblewidgets_p.h"
#include "qaccessiblemenu_p.h"
#include "private/qwidget_p.h"
#include "simplewidgets_p.h"
#include "rangecontrols_p.h"
#include "complexwidgets_p.h"
#include "itemviews_p.h"

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtreeview.h>
#include <qvariant.h>
#include <qaccessible.h>

#ifndef QT_NO_ACCESSIBILITY

QT_BEGIN_NAMESPACE

QAccessibleInterface *qAccessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *iface = 0;
    if (!object || !object->isWidgetType())
        return iface;

    QWidget *widget = static_cast<QWidget*>(object);
    // QWidget emits destroyed() from its destructor instead of letting the QObject
    // destructor do it, which means the QWidget is unregistered from the accessibillity
    // cache. But QWidget destruction also emits enter and leave events, which may end
    // up here, so we have to ensure that we don't fill the cache with an entry of
    // a widget that is going away.
    if (QWidgetPrivate::get(widget)->data.in_destructor)
        return iface;

    if (false) {
#ifndef QT_NO_LINEEDIT
    } else if (classname == QLatin1String("QLineEdit")) {
        if (widget->objectName() == QLatin1String("qt_spinbox_lineedit"))
            iface = 0;
        else
            iface = new QAccessibleLineEdit(widget);
#endif
#ifndef QT_NO_COMBOBOX
    } else if (classname == QLatin1String("QComboBox")) {
        iface = new QAccessibleComboBox(widget);
#endif
#ifndef QT_NO_SPINBOX
    } else if (classname == QLatin1String("QAbstractSpinBox")) {
        iface = new QAccessibleAbstractSpinBox(widget);
    } else if (classname == QLatin1String("QSpinBox")) {
        iface = new QAccessibleSpinBox(widget);
    } else if (classname == QLatin1String("QDoubleSpinBox")) {
        iface = new QAccessibleDoubleSpinBox(widget);
#endif
#ifndef QT_NO_SCROLLBAR
    } else if (classname == QLatin1String("QScrollBar")) {
        iface = new QAccessibleScrollBar(widget);
#endif
#ifndef QT_NO_SLIDER
    } else if (classname == QLatin1String("QAbstractSlider")) {
        iface = new QAccessibleAbstractSlider(widget);
    } else if (classname == QLatin1String("QSlider")) {
        iface = new QAccessibleSlider(widget);
#endif
#ifndef QT_NO_TOOLBUTTON
    } else if (classname == QLatin1String("QToolButton")) {
        iface = new QAccessibleToolButton(widget);
#endif // QT_NO_TOOLBUTTON
    } else if (classname == QLatin1String("QCheckBox")
            || classname == QLatin1String("QRadioButton")
            || classname == QLatin1String("QPushButton")
            || classname == QLatin1String("QAbstractButton")) {
        iface = new QAccessibleButton(widget);
    } else if (classname == QLatin1String("QDialog")) {
        iface = new QAccessibleWidget(widget, QAccessible::Dialog);
    } else if (classname == QLatin1String("QMessageBox")) {
        iface = new QAccessibleWidget(widget, QAccessible::AlertMessage);
#ifndef QT_NO_MAINWINDOW
    } else if (classname == QLatin1String("QMainWindow")) {
        iface = new QAccessibleMainWindow(widget);
#endif
    } else if (classname == QLatin1String("QLabel") || classname == QLatin1String("QLCDNumber")) {
        iface = new QAccessibleDisplay(widget);
#ifndef QT_NO_GROUPBOX
    } else if (classname == QLatin1String("QGroupBox")) {
        iface = new QAccessibleGroupBox(widget);
#endif
    } else if (classname == QLatin1String("QStatusBar")) {
        iface = new QAccessibleDisplay(widget);
#ifndef QT_NO_PROGRESSBAR
    } else if (classname == QLatin1String("QProgressBar")) {
        iface = new QAccessibleProgressBar(widget);
#endif
    } else if (classname == QLatin1String("QToolBar")) {
        iface = new QAccessibleWidget(widget, QAccessible::ToolBar, widget->windowTitle());
#ifndef QT_NO_MENUBAR
    } else if (classname == QLatin1String("QMenuBar")) {
        iface = new QAccessibleMenuBar(widget);
#endif
#ifndef QT_NO_MENU
    } else if (classname == QLatin1String("QMenu")) {
        iface = new QAccessibleMenu(widget);
#endif
#ifndef QT_NO_TREEVIEW
    } else if (classname == QLatin1String("QTreeView")) {
        iface = new QAccessibleTree(widget);
#endif // QT_NO_TREEVIEW
#ifndef QT_NO_ITEMVIEWS
    } else if (classname == QLatin1String("QTableView") || classname == QLatin1String("QListView")) {
        iface = new QAccessibleTable(widget);
    // ### This should be cleaned up. We return the parent for the scrollarea to hide it.
#endif // QT_NO_ITEMVIEWS
#ifndef QT_NO_TABBAR
    } else if (classname == QLatin1String("QTabBar")) {
        iface = new QAccessibleTabBar(widget);
#endif
    } else if (classname == QLatin1String("QSizeGrip")) {
        iface = new QAccessibleWidget(widget, QAccessible::Grip);
#ifndef QT_NO_SPLITTER
    } else if (classname == QLatin1String("QSplitter")) {
        iface = new QAccessibleWidget(widget, QAccessible::Splitter);
    } else if (classname == QLatin1String("QSplitterHandle")) {
        iface = new QAccessibleWidget(widget, QAccessible::Grip);
#endif
#if !defined(QT_NO_TEXTEDIT) && !defined(QT_NO_CURSOR)
    } else if (classname == QLatin1String("QTextEdit")) {
        iface = new QAccessibleTextEdit(widget);
    } else if (classname == QLatin1String("QPlainTextEdit")) {
        iface = new QAccessiblePlainTextEdit(widget);
#endif
    } else if (classname == QLatin1String("QTipLabel")) {
        iface = new QAccessibleDisplay(widget, QAccessible::ToolTip);
    } else if (classname == QLatin1String("QFrame")) {
        iface = new QAccessibleWidget(widget, QAccessible::Border);
#ifndef QT_NO_STACKEDWIDGET
    } else if (classname == QLatin1String("QStackedWidget")) {
        iface = new QAccessibleStackedWidget(widget);
#endif
#ifndef QT_NO_TOOLBOX
    } else if (classname == QLatin1String("QToolBox")) {
        iface = new QAccessibleToolBox(widget);
#endif
#ifndef QT_NO_MDIAREA
    } else if (classname == QLatin1String("QMdiArea")) {
        iface = new QAccessibleMdiArea(widget);
    } else if (classname == QLatin1String("QMdiSubWindow")) {
        iface = new QAccessibleMdiSubWindow(widget);
#endif
#if QT_CONFIG(dialogbuttonbox)
    } else if (classname == QLatin1String("QDialogButtonBox")) {
        iface = new QAccessibleDialogButtonBox(widget);
#endif
#ifndef QT_NO_DIAL
    } else if (classname == QLatin1String("QDial")) {
        iface = new QAccessibleDial(widget);
#endif
#ifndef QT_NO_RUBBERBAND
    } else if (classname == QLatin1String("QRubberBand")) {
        iface = new QAccessibleWidget(widget, QAccessible::Border);
#endif
#if !defined(QT_NO_TEXTBROWSER) && !defined(QT_NO_CURSOR)
    } else if (classname == QLatin1String("QTextBrowser")) {
        iface = new QAccessibleTextBrowser(widget);
#endif
#ifndef QT_NO_SCROLLAREA
    } else if (classname == QLatin1String("QAbstractScrollArea")) {
        iface = new QAccessibleAbstractScrollArea(widget);
    } else if (classname == QLatin1String("QScrollArea")) {
        iface = new QAccessibleScrollArea(widget);
#endif
#ifndef QT_NO_CALENDARWIDGET
    } else if (classname == QLatin1String("QCalendarWidget")) {
        iface = new QAccessibleCalendarWidget(widget);
#endif
#ifndef QT_NO_DOCKWIDGET
    } else if (classname == QLatin1String("QDockWidget")) {
        iface = new QAccessibleDockWidget(widget);
#endif

    } else if (classname == QLatin1String("QDesktopScreenWidget")) {
        iface = 0;
    } else if (classname == QLatin1String("QWidget")) {
        iface = new QAccessibleWidget(widget);
    } else if (classname == QLatin1String("QWindowContainer")) {
        iface = new QAccessibleWindowContainer(widget);
    }

    return iface;
}

QT_END_NAMESPACE

#endif // QT_NO_ACCESSIBILITY
