#include "statistics.h"
#include "./ui_statistics.h"

statistics::statistics(QWidget *parent)
    : QWidget(parent), ui(new Ui::statistics)
{
    ui->setupUi(this);

    // 设置界面上文件拖拽的属性
    this->setAcceptDrops(true);
    on_pbn_clear_result_clicked();

    initForm();
}

statistics::~statistics()
{
    delete ui;
}

void statistics::initForm()
{
    resize(1000, 800);

    // 设置表格样式和形状
    QStringList head_text;
    head_text << "文件名"
              << "类型"
              << "大小"
              << "总行数"
              << "代码行数"
              << "注释行数"
              << "空白行数"
              << "路径";
    QList<int> column_width;
    column_width << 130 << 50 << 70 << 80 << 70 << 70 << 70 << 150;
    int num_column = head_text.size();

    ui->tbw_show_message->setColumnCount(num_column);
    // 设置行表头各列的标题
    ui->tbw_show_message->setHorizontalHeaderLabels(head_text);
    // 选择模式，设置选择方式为行选择
    ui->tbw_show_message->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置表格不允许编辑
    ui->tbw_show_message->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 单选
    ui->tbw_show_message->setSelectionMode(QAbstractItemView::SingleSelection);
    // verticalHeader()获取列表头，setVisible(false)设置不可见
    ui->tbw_show_message->verticalHeader()->setVisible(false);
    // 设置默认行高20
    ui->tbw_show_message->verticalHeader()->setDefaultSectionSize(20);
    // 设置点击时的效果
    ui->tbw_show_message->verticalHeader()->setHighlightSections(false);
    // horizontalHeader()获取行表头，setStretchLastSection()设置宽度自适应调整
    ui->tbw_show_message->horizontalHeader()->setStretchLastSection(true);
    ui->tbw_show_message->horizontalHeader()->setHighlightSections(false);

    for (int i = 0; i < num_column; i++) {
        ui->tbw_show_message->setColumnWidth(i, column_width.at(i));
    }

    // 设置字体颜色
    ui->le_num_file->setStyleSheet("color:#17A086");
    ui->le_num_byte->setStyleSheet("color:#17A086");
    ui->le_num_line->setStyleSheet("color:#17A086");
    ui->le_line_code->setStyleSheet("color:#17A086");
    ui->le_line_note->setStyleSheet("color:#17A086");
    ui->le_line_blank->setStyleSheet("color:#17A086");

    // 设置字体大小
    QFont font;
    // 字体加粗
    font.setBold(true);
    font.setPixelSize(18);
    ui->le_num_file->setFont(font);
    ui->le_num_byte->setFont(font);
    ui->le_num_line->setFont(font);
    ui->le_line_code->setFont(font);
    ui->le_line_note->setFont(font);
    ui->le_line_blank->setFont(font);

    // 提示信息
    ui->le_select_type->setPlaceholderText("例如：*.h *.c *.cpp");
}

bool statistics::checkFile(const QString &file_name)
{
    QFileInfo file(file_name);
    // suffix(): 最后一个后缀（从文件名最后一个.分隔符开始）
    QString suffix = "*." + file.suffix();
    // trimmed(): 去除了开头和结尾的空白字符串
    QString filter = ui->le_select_type->text().trimmed();
    QStringList filters = filter.split(" ");
    // 如：filters = ("*.h", "*.cpp", "*.c")

    return filters.contains(suffix);
}

void statistics::countPathFileNumber(const QString &file_path)
{
    QDir dir(file_path);
    // entryInfoList():列出dir(path)目录文件下所有文件和目录信息，存储到file_list容器
    QFileInfoList files_info = dir.entryInfoList();

    // 遍历指定路径下的所有文件，符合过滤要求的文件存储在list_files容器
    foreach (QFileInfo file_info, files_info) {
        QString file_name = file_info.fileName();
        if (file_info.isFile()) {
            if (checkFile(file_name)) {
                list_files << file_info.filePath();
            }
        } else {
            // 如果不是文件，.表示当前目录，..表示上级目录，就继续遍历
            if (file_name == "." || file_name == "..") {
                continue;
            }
            countPathFileNumber(file_info.absoluteFilePath());
        }
    }
}

void statistics::countFileRows(const QStringList &files)
{
    /*设置信息*/

    // 代码行数、空白行数、注释行数
    int num_code_line;
    int num_blank_line;
    int num_note_line;
    // 文件数量
    int num_files = files.count();

    on_pbn_clear_result_clicked();
    ui->tbw_show_message->setRowCount(num_files);

    quint32 total_lines = 0;
    quint32 total_bytes = 0;
    quint32 total_codes = 0;
    quint32 total_notes = 0;
    quint32 total_blank = 0;

    for (int i = 0; i < num_files; i++) {
        QFileInfo file_info(files.at(i));
        countCodeRows(file_info.filePath(), num_code_line, num_blank_line,
                      num_note_line);
        // 总行数
        int num_line_all = num_code_line + num_blank_line + num_note_line;

        QTableWidgetItem *item_name = new QTableWidgetItem;
        item_name->setText(file_info.fileName());
        QTableWidgetItem *item_suffix = new QTableWidgetItem;
        item_suffix->setText(file_info.suffix());
        QTableWidgetItem *item_size = new QTableWidgetItem;
        item_size->setText(QString::number(file_info.size()));
        QTableWidgetItem *item_line = new QTableWidgetItem;
        item_line->setText(QString::number(num_line_all));
        QTableWidgetItem *item_code = new QTableWidgetItem;
        item_code->setText(QString::number(num_code_line));
        QTableWidgetItem *item_note = new QTableWidgetItem;
        item_note->setText(QString::number(num_note_line));
        QTableWidgetItem *item_blank = new QTableWidgetItem;
        item_blank->setText(QString::number(num_blank_line));
        QTableWidgetItem *item_path = new QTableWidgetItem;
        item_path->setText(file_info.filePath());

        item_name->setTextAlignment(Qt::AlignCenter);
        item_suffix->setTextAlignment(Qt::AlignCenter);
        item_size->setTextAlignment(Qt::AlignCenter);
        item_line->setTextAlignment(Qt::AlignCenter);
        item_code->setTextAlignment(Qt::AlignCenter);
        item_note->setTextAlignment(Qt::AlignCenter);
        item_blank->setTextAlignment(Qt::AlignCenter);
        item_path->setTextAlignment(Qt::AlignCenter);

        ui->tbw_show_message->setItem(i, 0, item_name);
        ui->tbw_show_message->setItem(i, 1, item_suffix);
        ui->tbw_show_message->setItem(i, 2, item_size);
        ui->tbw_show_message->setItem(i, 3, item_line);
        ui->tbw_show_message->setItem(i, 4, item_code);
        ui->tbw_show_message->setItem(i, 5, item_note);
        ui->tbw_show_message->setItem(i, 6, item_blank);
        ui->tbw_show_message->setItem(i, 7, item_path);

        total_bytes += file_info.size();
        total_lines += num_line_all;
        total_codes += num_code_line;
        total_notes += num_note_line;
        total_blank += num_blank_line;

        if (i % 100 == 0) {
            qApp->processEvents();
        }
    }

    // 显示结果
    list_files.clear();

    ui->le_num_file->setText(QString::number(num_files));
    ui->le_num_line->setText(QString::number(total_lines));
    ui->le_line_note->setText(QString::number(total_notes));
    ui->le_line_blank->setText(QString::number(total_blank));

    QString suf = "Bytes";
    if (total_bytes > 1024) {
        total_bytes = total_bytes / 1024; // KB
        suf = "KB";
    }
    if (total_bytes > 1024) {
        total_bytes = total_bytes / 1024; // MB
        suf = "MB";
    }
    if (total_bytes > 1024) {
        total_bytes = total_bytes / 1024; // GB
        suf = "GB";
    }

    ui->le_num_byte->setText(QString::number(total_bytes) + suf);
    ui->le_line_code->setText(QString::number(total_codes));

    // 计算百分比
    double percent = 0.0;
    percent = ((double)total_codes / total_lines) * 100;
    ui->lbl_percent_code_row->setText(
        QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
    percent = ((double)total_blank / total_lines) * 100;
    ui->lbl_percent_blank_row->setText(
        QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
    percent = ((double)total_notes / total_lines) * 100;
    ui->lbl_percent_note_row->setText(
        QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
}

void statistics::countCodeRows(const QString &file_name, int &num_line_code,
                               int &num_line_blank, int &num_line_note)
{
    // 实际计算
    num_line_code = num_line_blank = num_line_note = 0;
    QFile file(file_name);
    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        QString line;
        bool is_note = false;
        while (!in.atEnd()) {
            line = in.readLine();
            // 移除空行
            if (line.startsWith(" ")) {
                line.remove(" ");
            }
            if (line.startsWith("/*")) {
                is_note = true;
            }

            // 是否是注释
            if (is_note) {
                num_line_note++;
            } else {
                if (line.startsWith("//")) {
                    num_line_note++;
                } else if (line.isEmpty()) {
                    num_line_blank++;
                } else {
                    num_line_code++;
                }
            }

            if (line.endsWith("*/")) {
                is_note = false;
            }
        }
    }
}

void statistics::dragEnterEvent(QDragEnterEvent *event)
{
    // MIME 消息能包含文本、图像、音频、视频以及其他应用程序专用的数据
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void statistics::dropEvent(QDropEvent *event)
{
    const QMimeData *mine_data = event->mimeData();
    list_files.clear();

    if (mine_data->hasUrls()) {
        QList<QUrl> url_list = mine_data->urls();
        QFileInfo file_info;
        for (int i = 0; i < url_list.size(); i++) {
            QString file_path = url_list.at(i).toLocalFile();
            file_info.setFile(file_path);
            // 如果是文件夹
            if (file_info.isDir()) {
                countPathFileNumber(file_path);
            } else {
                // 不是文件夹，就是正常的文件
                if (checkFile(file_path)) {
                    list_files << file_path;
                }
            }
        }
    }
    countFileRows(list_files);
}

void statistics::on_pbn_open_file_clicked()
{
    QString filter =
        QString("代码后缀(%1)").arg(ui->le_file_name->text().trimmed());
    QStringList files =
        QFileDialog::getOpenFileNames(this, "选择文件", "./", filter);
    if (filter.size() > 0) {
        ui->le_file_name->setText(files.join("|"));
        countFileRows(files);
    }
}

void statistics::on_pbn_open_catalog_clicked()
{
    QString path = QFileDialog::getExistingDirectory(
        this, "选择目录", "./",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!path.isEmpty()) {
        ui->le_catalog_name->setText(path);
        list_files.clear();
        countPathFileNumber(path);
        countFileRows(list_files);
    }
}

void statistics::on_pbn_clear_result_clicked()
{
    ui->le_num_file->setText("0");
    ui->le_num_byte->setText("0");
    ui->le_num_line->setText("0");
    ui->le_line_code->setText("0");
    ui->le_line_note->setText("0");
    ui->le_line_blank->setText("0");

    ui->tbw_show_message->setRowCount(0);

    ui->lbl_percent_code_row->setText("00.00%");
    ui->lbl_percent_note_row->setText("00.00%");
    ui->lbl_percent_blank_row->setText("00.00%");
}
