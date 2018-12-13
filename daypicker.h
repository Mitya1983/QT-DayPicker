#ifndef DAYPICKER_H
#define DAYPICKER_H

#include <QWidget>
#include <QFont>
#include <QWheelEvent>
#include <vector>
#include <memory>


namespace Ui {
class DayPicker;
}

class DayPicker : public QWidget
{
    Q_OBJECT

public:
    explicit DayPicker(QWidget *parent = nullptr);
    int day();
    ~DayPicker() override = default;

private:

    Ui::DayPicker *ui;

//  Curently selected day
    int selectedDay;

//  Number of days. Is set by setNumberofDays
    int maxDay;

//  Used for different font sizes of shown days
    QFont selectedFont;
    QFont day1Font;
    QFont day2Font;

//  Stores day values
    std::unique_ptr<std::vector<int>> days;

//  Functions used to set previous and next day, taking into account that days are shown circle like
    int previousIndex(int _curentIndex);
    int nextIndex(int _curentIndex);

//  Scroling
//  Used for scroll accumulation
    int upCount = 0, downCount = 0;

//  Used to store initital mouse cursor x position
    int currentCursorXPos = 0;


    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;



private slots:

//  Days vector initialisation and fillment
    void createDays(int _days);

//  Set shown days +- 2 days in respect of current/selected date
    void setDaysShown(int _day);

signals:

//  Used to invoke createDays each time when maxDay is changed (changing 28, 29, 30 and 31 days count)
    void onMaxDayChanged(int newMaxDayValue);

//  Used to invoke setDayesShown
    void onSelectedDayChanged(int newSelectedDay);

public slots:

//  Setting number of days in respect of months and year (leap year)
    void setNumberOfDays(const int &month, const int &year);
};

#endif // DAYPICKER_H
