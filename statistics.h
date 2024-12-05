#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
namespace Ui {
class statistics;
}
QT_END_NAMESPACE

class statistics : public QWidget
{
    Q_OBJECT

public:
    statistics(QWidget *parent = nullptr);
    ~statistics();

private slots:
    void on_pbn_open_file_clicked();

    void on_pbn_open_catalog_clicked();

    void on_pbn_clear_result_clicked();

private:
    void initForm();
    /// 检查文件是否符合过滤的文件要求
    bool checkFile(const QString &file_name);
    /// 计算指定路径中文件的数量
    void countPathFileNumber(const QString &file_path);
    //    /// 计算总的文件数量（用于拖拽到列表中）
    //    void countFileNumber(const QStringList &files_name);
    /// 计算文件的行数
    void countFileRows(const QStringList &files);
    /// 计算代码的行数
    /// 参数：文件名、代码行数、空白行数、注释行数
    void countCodeRows(const QString &file_name, int &num_line_code,
                       int &num_line_blank, int &num_line_note);

    // 文件拖拽事件
    // 拖动文件进入到列表
    void dragEnterEvent(QDragEnterEvent *event);
    // 放下文件到列表中
    void dropEvent(QDropEvent *event);

private:
    Ui::statistics *ui;

private:
    QStringList list_files{};
};
#endif // STATISTICS_H
