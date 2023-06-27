#ifndef SHARE_FILE_WIDGET_H
#define SHARE_FILE_WIDGET_H

#include <QWidget>

namespace Ui {
class Share_File_Widget;
}

class Share_File_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Share_File_Widget(QWidget *parent = nullptr);
    ~Share_File_Widget();
    void Flush_Shared_Page();
    void DownLoad_Shared_File();

signals:
    void Flush_Page();

private:
    Ui::Share_File_Widget *ui;
};

#endif // SHARE_FILE_WIDGET_H
