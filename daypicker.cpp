#include <QDate>
#include <QDebug>
#include <QPoint>
#include "daypicker.h"
#include "ui_daypicker.h"

DayPicker::DayPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayPicker)
{
    ui->setupUi(this);
    connect(this, &DayPicker::onMaxDayChanged, this, &DayPicker::createDays);
    connect(this, &DayPicker::onSelectedDayChanged, this, &DayPicker::setDaysShown);
    selectedDay = QDate::currentDate().day();
    setNumberOfDays(QDate::currentDate().month(), QDate::currentDate().year());
    this->setMinimumHeight((this->height() / 15)  *  10);
    selectedFont = ui->selectedDay->font();
    day1Font = ui->dayMinus1->font();
    day2Font = ui->dayMinus2->font();
    selectedFont.setPointSize(this->height() / 15);
    day1Font.setPointSize(this->height() / 20);
    day2Font.setPointSize(this->height() / 22);
    ui->selectedDay->setFont(selectedFont);
    ui->dayMinus1->setFont(day1Font);
    ui->dayPlus1->setFont(day1Font);
    ui->dayMinus2->setFont(day2Font);
    ui->dayPlus2->setFont(day2Font);
    setDaysShown(selectedDay);
}

int DayPicker::day()
{
    return selectedDay;
}

int DayPicker::previousIndex(int _curentIndex)
{
    if (_curentIndex > 0)
        --_curentIndex;
    else
        _curentIndex = maxDay - 1;
    return _curentIndex;
}

int DayPicker::nextIndex(int _currentIndex)
{
    if (_currentIndex < maxDay - 1)
        ++_currentIndex;
    else
        _currentIndex = 0;
    return _currentIndex;
}

void DayPicker::wheelEvent(QWheelEvent *event)
{

    if (event->delta() > 0)
    {
        upCount += event->delta();
        if (upCount >= 80)
        {
            if (selectedDay > 0)
                selectedDay--;
            if (selectedDay == 0)
                selectedDay = maxDay;
            upCount = 0;
            emit onSelectedDayChanged(selectedDay);
        }
    }
    if (event->delta() < 0)
    {
        downCount -= event->delta();
        if (downCount >= 80)
        {
            if (selectedDay < maxDay)
                selectedDay++;
            if (selectedDay == maxDay)
                selectedDay = 1;
            downCount = 0;
            emit onSelectedDayChanged(selectedDay);
        }
    }
    event->accept();
}

void DayPicker::mousePressEvent(QMouseEvent *event)
{
    currentCursorXPos = event->pos().y();
}

void DayPicker::mouseMoveEvent(QMouseEvent *event)
{
    if ((geometry().contains(QCursor::pos())) && (event->buttons() & Qt::LeftButton))
    {
        if (event->pos().y() < currentCursorXPos)
        {
            upCount += 2;
            if  (upCount >= height() / maxDay)
            {
                currentCursorXPos = event->pos().y();
                if (selectedDay < maxDay)
                    selectedDay++;
                if (selectedDay == maxDay)
                    selectedDay = 1;
                upCount = 0;
                emit onSelectedDayChanged(selectedDay);
            }
        }
        if (event->pos().y() > currentCursorXPos)
        {
            downCount += 2;
            if (downCount >= height() / maxDay)
            {
                currentCursorXPos = event->pos().y();
                if (selectedDay > 0)
                    selectedDay--;
                if (selectedDay == 0)
                    selectedDay = maxDay;
                downCount = 0;
                emit onSelectedDayChanged(selectedDay);
            }
        }
    }
}

void DayPicker::createDays(int _days)
{
    days = std::make_unique<std::vector<int>>(_days);
    days->shrink_to_fit();
    for (int i = 0; i < _days; i++)
        (*days)[static_cast<unsigned long>(i)] = i + 1;
}

void DayPicker::setDaysShown(int _day)
{
    --_day;
    ui->selectedDay->setNum(days->at(static_cast<unsigned long>(_day)));
    ui->dayPlus1->setNum(days->at(static_cast<unsigned long>(nextIndex(_day))));
    ui->dayPlus2->setNum(days->at(static_cast<unsigned long>(nextIndex(nextIndex(_day)))));
    ui->dayMinus1->setNum(days->at(static_cast<unsigned long>(previousIndex(_day))));
    ui->dayMinus2->setNum(days->at(static_cast<unsigned long>(previousIndex(previousIndex(_day)))));
}

void DayPicker::setNumberOfDays(const int &month, const int &year)
{
    switch(month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
    {
        maxDay = 31;
        break;
    }
    case 4:
    case 6:
    case 9:
    case 11:
    {
        maxDay = 30;
        break;
    }
    case 2:
    {
        if (!QDate::isLeapYear(year))
            maxDay = 28;
        else
            maxDay = 29;
        break;

    }
    }
    emit onMaxDayChanged(maxDay);
}
