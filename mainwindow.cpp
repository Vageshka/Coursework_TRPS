#include "mainwindow.h"

#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QToolButton>
#include <QDebug>
#include <QColorDialog>
#include <QPushButton>
#include <QFile>
#include <dgrmreaderwriter.h>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbars();
    createToolBox();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);

    view = new QGraphicsView();
    updateScene();

    layout->addWidget(view);
    stylesTabs = new QTabWidget;
    stylesTabs->addTab(createPaperTab(), "Paper");
    stylesTabs->addTab(createItemTab(), "Item");
    stylesTabs->addTab(createArrowTab(), "Arrow");
    layout->addWidget(stylesTabs);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("Redactor"));
}

void MainWindow::save()
{
    if(filename_.isEmpty()) {
        filename_ = QFileDialog::getSaveFileName(this, "Choose file", QString(),
                                                 "Diagram file (*.dgrm)");
        if(filename_.isEmpty()){
            return;
        }
    }

    IReaderWriter *rw = new DGRMReaderWriter;
    handleReadWriteErrors(rw->write(filename_, scene));
}

void MainWindow::openFile()
{
    if(!sceneIsSaved) {
        switch (askForSave()) {
        case QMessageBox::Save: {
            save();
            break;
        }
        case QMessageBox::Discard: {
            break;
        }
        case QMessageBox::Cancel:
        default:
            return;
        }
    }
    filename_ = QFileDialog::getOpenFileName(this, "Choose file", QString(),
                                             "Diagram file (*.dgrm)");
    if(filename_.isEmpty()){
        return;
    }

    IReaderWriter *rw = new DGRMReaderWriter;
    updateScene();
    handleReadWriteErrors(rw->read(filename_, scene));
}

void MainWindow::creatNew()
{
    if(scene->isClear()) {
        return;
    }
    if(!sceneIsSaved) {
        switch (askForSave()) {
        case QMessageBox::Save: {
            save();
        }
        case QMessageBox::Discard: {
            updateScene();
            break;
        }
        case QMessageBox::Cancel:
        default:
            return;
        }
    } else updateScene();
}

void MainWindow::saveAs()
{
    qDebug() << filename_;
    if(filename_.isEmpty()){
        save();
    } else {
        auto old_name = filename_;
        filename_ = "";
        save();
        if(filename_.isEmpty()) {
            filename_ = old_name;
        }
    }
}

//This method create action for menu bar and tool bar
void MainWindow::createActions()
{
    createAction = new QAction(/*QIcon("."),*/"Create new", this);
    createAction->setShortcut(tr("Ctrl+N"));
    createAction->setStatusTip("Creating a new diagram...");
    connect(createAction, &QAction::triggered, this, &MainWindow::creatNew);

    openAction = new QAction("Open", this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip("Opening diagram");
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    saveAction = new QAction("Save", this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip("Saving...");
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    saveAsAction = new QAction("Save as", this);
    saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
    saveAsAction->setStatusTip("Saving...");
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    previewAction = new QAction("Preview PDF", this);
    connect(previewAction, &QAction::triggered, this, &MainWindow::opentPreviewDialog);

    exportPdfAction = new QAction("Export PDF", this);
    connect(exportPdfAction, &QAction::triggered, this, [&] {
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize((QPrinter::PageSize)scene->paper->size());
        printer.setOrientation((QPrinter::Orientation)scene->paper->orientation());
        printer.setOutputFormat(QPrinter::PdfFormat);

        auto filename = QFileDialog::getSaveFileName(this, "Choose file", QString(),
                                                 "PDF file (*.pdf)");
        printer.setOutputFileName(filename);

        QPainter p(&printer);

        QRectF old_rect = scene->sceneRect();
        scene->setSceneRect(scene->paper->rect());
        scene->clearSelection();
        scene->render(&p);
        scene->setSceneRect(old_rect);
        view->centerOn(scene->paper);
        p.end();
    });

    exportPngAction = new QAction("Export PNG", this);
    connect(exportPngAction, &QAction::triggered, this, [&] {
        QImage img(scene->paper->rect().size().toSize(), QImage::Format_ARGB32_Premultiplied);

        auto filename = QFileDialog::getSaveFileName(this, "Choose file", QString(),
                                                 "PDF file (*.png)");
        QPainter p(&img);

        QRectF old_rect = scene->sceneRect();
        scene->setSceneRect(scene->paper->rect());
        scene->clearSelection();
        scene->render(&p);
        scene->setSceneRect(old_rect);
        view->centerOn(scene->paper);
        p.end();
        img.save(filename);
    });

    aboutAction = new QAction("About this programm", this);

    deleteAction = new QAction("Delete", this);
    deleteAction->setShortcut(tr("Del"));
    deleteAction->setStatusTip("Deleting");
    deleteAction->setIcon(QIcon(tr(":/assets/trash.png")));
    connect(deleteAction, &QAction::triggered, this, [&] {
        foreach(auto i, scene->selectedItems()) {
            scene->removeItem(i);
        }
        scene->updateArrowsItems();
    });

    boldAction = new QAction("Bold", this);
    boldAction->setCheckable(true);
    boldAction->setIcon(QIcon(":/assets/bold.png"));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, &QAction::triggered, this, [&]() {
        QFont new_font = scene->font();
        new_font.setBold(boldAction->isChecked());
        scene->setFont(new_font);

        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                QFont tmp_font = item->font();
                tmp_font.setBold(boldAction->isChecked());
                item->setFont(tmp_font);
            }
        }
    });

    italicAction = new QAction("Italic", this);
    italicAction->setCheckable(true);
    italicAction->setIcon(QIcon(":/assets/italics.png"));
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, &QAction::triggered, this, [&]() {
        QFont new_font = scene->font();
        new_font.setItalic(italicAction->isChecked());
        scene->setFont(new_font);

        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                QFont tmp_font = item->font();
                tmp_font.setItalic(italicAction->isChecked());
                item->setFont(tmp_font);
            }
        }
    });


    underlineAction = new QAction("Underline", this);
    underlineAction->setCheckable(true);
    underlineAction->setIcon(QIcon(":/assets/underlined-text.png"));
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, &QAction::triggered, this, [&]() {
        QFont new_font = scene->font();
        new_font.setUnderline(underlineAction->isChecked());
        scene->setFont(new_font);

        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                QFont tmp_font = item->font();
                tmp_font.setUnderline(underlineAction->isChecked());
                item->setFont(tmp_font);
            }
        }
    });

    textColorAction = new QAction("Text color");
    textColorAction->setIcon(QIcon(":/assets/color-text.png"));
    connect(textColorAction, &QAction::triggered, this, [&] {
        auto items = scene->selectedDiagramItems();
        if(items.size() == 0) {
            return;
        }

        QColor cl = QColorDialog::getColor(items[0]->textColor(), this, "Choose color");
        if (cl.isValid()) {
            foreach(auto item, items) {
                item->setTextColor(cl);
            }
        }
    });
}

//Creating a menu bar
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(createAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(previewAction);
    exportMenu = fileMenu->addMenu("Export");
    exportMenu->addAction(exportPdfAction);
    exportMenu->addAction(exportPngAction);

    aboutMenu = menuBar()->addMenu("Help");
    aboutMenu->addAction(aboutAction);

}

//This function creats tool bars
void MainWindow::createToolbars()
{
    //Creating fit scene width into view button
    QToolButton *fitWidthButton = new QToolButton;
    fitWidthButton->setIcon(QIcon(":/assets/fit_width.png"));
    connect(fitWidthButton, &QToolButton::clicked, this, [&] {
        view->fitInView(scene->paper->rect()
                        .adjusted(-50,0,50,-2*scene->paper->rect().height()/3)
                        .normalized(),
                        Qt::KeepAspectRatio);
        setComboCurScale();
    });
    //Creating fit scene page into view button
    QToolButton *fitPageButton = new QToolButton;
    fitPageButton->setIcon(QIcon(":/assets/fit_page.png"));
    connect(fitPageButton, &QToolButton::clicked, this, [&] {
        view->fitInView(scene->paper->rect(), Qt::KeepAspectRatio);
        setComboCurScale();
    });

    scaleToolBar = addToolBar(tr("Scale bar"));
    scaleToolBar->addWidget(fitWidthButton);
    scaleToolBar->addWidget(fitPageButton);
    scaleToolBar->addSeparator();

    //Creating scene scale tool box
    sceneScaleCombo = new QComboBox;
    sceneScaleCombo->setEditable(true);
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged, this, [&] (const QString &scale) {
        double newScale = scale.leftRef(scale.indexOf(tr("%"))).toDouble() / 100.0;
        view->resetTransform();
        view->scale(newScale, newScale);
    });
    //Create increase scale button
    QToolButton *increaseScaleButton = new QToolButton;
    increaseScaleButton->setIcon(QIcon(":/assets/plus-button.png"));
    connect(increaseScaleButton, &QToolButton::clicked, this, [&] {
        view->scale(1.15, 1.15);
        setComboCurScale();
    });
    //Create decrease scale button
    QToolButton *decreaseScaleButton = new QToolButton;
    decreaseScaleButton->setIcon(QIcon(":/assets/minus-button.png"));
    connect(decreaseScaleButton, &QToolButton::clicked, this, [&] {
        view->scale(0.85, 0.85);
        setComboCurScale();
    });
    scaleToolBar->addWidget(sceneScaleCombo);
    scaleToolBar->addWidget(increaseScaleButton);
    scaleToolBar->addWidget(decreaseScaleButton);

    //Creating cursor button
    QToolButton *cursorButton = new QToolButton;
    cursorButton->setCheckable(true);
    cursorButton->setChecked(true);

    cursorButton->setIcon(QIcon(":/assets/cursor.png"));

    QToolButton *lineButton = new QToolButton;
    lineButton->setCheckable(true);
    lineButton->setIcon(QIcon(":/assets/diagonal_arrow.png"));

    actionTypeGroup = new QButtonGroup(this);
    actionTypeGroup->addButton(cursorButton, int(DiagramScene::MoveItem));
    actionTypeGroup->addButton(lineButton, int(DiagramScene::AddArrow));
    actionTypeGroup->setExclusive(true);
    connect(actionTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, [&](QAbstractButton *button) {
        if(auto checked = buttonGroup->checkedButton()) {
            buttonGroup->setExclusive(false);
            checked->setChecked(false);
            buttonGroup->setExclusive(true);
        }
        scene->setState(static_cast<DiagramScene::Mode>(actionTypeGroup->id(button)));
    });

    arrowheadCombo = createArrowheadsCombo();
    arrowheadCombo->setCurrentIndex(1);
    connect(arrowheadCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, [&] (int index) {
        scene->setArrowheadType(static_cast<Arrow::ArrowheadType>(index));
        foreach(auto i, scene->selectedArrows()) {
            i->setArrowhead(static_cast<Arrow::ArrowheadType>(index));
        }
    });

    defaultToolBar = addToolBar(tr("Pointer type"));
    defaultToolBar->addWidget(cursorButton);
    defaultToolBar->addWidget(lineButton);
    defaultToolBar->addWidget(arrowheadCombo);
    defaultToolBar->addAction(deleteAction);

    //Creating Text toolbar
    fontCombo = new QFontComboBox;
    fontCombo->setCurrentFont(QFont("Times New Roman"));
    fontCombo->setMaximumWidth(200);
    connect(fontCombo, &QFontComboBox::currentFontChanged, this, [&] (const QFont &f) {
        QFont new_font = scene->font();
        new_font.setFamily(f.family());
        scene->setFont(new_font);

        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                QFont tmp_font = item->font();
                tmp_font.setFamily(f.family());
                item->setFont(tmp_font);
            }
        }
    });

    fontSizeSpin = new QSpinBox;
    fontSizeSpin->setRange(1, 1000);
    fontSizeSpin->setValue(14);
    fontSizeSpin->setMaximumWidth(80);
    connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        QFont new_font = scene->font();
        new_font.setPixelSize(val);
        scene->setFont(new_font);

        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                QFont tmp_font = item->font();
                tmp_font.setPixelSize(val);
                item->setFont(tmp_font);
            }
        }
    });

    textToolBar = addToolBar(tr("Text"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeSpin);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->addAction(textColorAction);
}

void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    QGridLayout *layout = new QGridLayout;
    int colrow = 0;

    for(int type=DiagramItem::begin; type<=DiagramItem::end; type++) {
        switch (type) {
        case DiagramItem::Regular: {
            for(int subtype = RegularVertex::begin;
                    subtype <= RegularVertex::end; subtype++) {
                RegularVertex item((RegularVertex::VertexType)subtype);
                layout->addWidget(createToolBoxButton(item, type*1000+subtype),
                                    colrow/2, colrow % 2);
                colrow++;
            }
            break;
        }
        case DiagramItem::Container: {
            break;
        }
        }
    }
    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, [&](QAbstractButton *button) {
        if(auto checked = actionTypeGroup->checkedButton()) {
            actionTypeGroup->setExclusive(false);
            checked->setChecked(false);
            actionTypeGroup->setExclusive(true);
        }

        scene->setState(DiagramScene::AddItem);
        scene->setItemType(static_cast<DiagramItem::ItemType>(buttonGroup->id(button) / 1000));
        scene->setItemSubtype(buttonGroup->id(button) % 1000);
        qDebug() << buttonGroup->id(button);
//        scene->setItemTypeFlags(buttonGroup->id(button));
    });

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    //    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

QWidget *MainWindow::createPaperTab()
{
    QVBoxLayout *vl = new QVBoxLayout;
    QLabel *l_size = new QLabel;
    l_size->setText("Size");
    QLabel *l_orientation = new QLabel;
    l_orientation->setText("Orientation");

    paperSizeCombo = new QComboBox;
    for (int i = 0; i < 5; i++) {
        paperSizeCombo->addItem("A"+QString::number(i));
    }
    paperSizeCombo->setCurrentText("A4");
    connect(paperSizeCombo, &QComboBox::currentTextChanged, this, [&] (const QString &size) {
        scene->paper->setSize(scene->paper->strToPaperSize(size));
        view->fitInView(scene->paper->rect(), Qt::KeepAspectRatio);
        setComboCurScale();
    });
    paperOrientationCombo = new QComboBox;
    paperOrientationCombo->addItem("Portrait");
    paperOrientationCombo->addItem("Landscape");
    connect(paperOrientationCombo, &QComboBox::currentTextChanged, this, [&] (const QString &orientation) {
        scene->paper->setOrientation(
            orientation == "Portrait" ? Paper::Portrait : Paper::Landscape);
    });

    QPushButton *color_button = new QPushButton("Change color");
    connect(color_button, &QPushButton::clicked, this, [&] {
        QColor cl = QColorDialog::getColor(scene->paper->brush().color(), this, "Choose color");
        if (cl.isValid()) {
            scene->paper->setBrush(cl);
        }
    });

    vl->addWidget(l_size);
    vl->addWidget(paperSizeCombo);
    vl->addWidget(l_orientation);
    vl->addWidget(paperOrientationCombo);
    vl->addWidget(color_button);
    vl->addStretch();

    QWidget *paper_tab = new QWidget;
    paper_tab->setLayout(vl);
    return paper_tab;
}

QWidget *MainWindow::createItemTab()
{
    QVBoxLayout *vl = new QVBoxLayout;

    itemTextEdit = new QTextEdit;
    connect(itemTextEdit, &QTextEdit::textChanged, this, [&] {
        if(scene->selectedDiagramItems().size()) {
            foreach(auto item, scene->selectedDiagramItems()) {
                item->setText(itemTextEdit->toPlainText());
            }
        }
    });

    vl->addWidget(itemTextEdit);

    QPushButton *item_color = new QPushButton("Change item color");
    connect(item_color, &QPushButton::clicked, this, [&] {
        auto items = scene->selectedDiagramItems();
        if(items.size() == 0) {
            return;
        }

        QColor cl = QColorDialog::getColor(items[0]->Brush().color(),
                                            this, "Change item color");
        if (cl.isValid()) {
            for(auto item : items) {
                item->setBrush(cl);
            }
        }
    });

    widthSpin = new QSpinBox;
    widthSpin->setRange(50, 10000);
    widthSpin->setMaximumWidth(100);
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        for(auto i : scene->selectedDiagramItems()) {
            i->setWidth(val);
        }
    });

    heightSpin = new QSpinBox;
    heightSpin->setRange(50, 10000);
    heightSpin->setMaximumWidth(100);
    connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        for(auto i : scene->selectedDiagramItems()) {
            i->setHeight(val);
        }
    });

    QGridLayout *item_settings = new QGridLayout;
    item_settings->setHorizontalSpacing(10);
    item_settings->addWidget(new QLabel("Set width"), 0, 0);
    item_settings->addWidget(widthSpin, 0, 1);
    item_settings->addWidget(new QLabel("Set height"), 1, 0);
    item_settings->addWidget(heightSpin, 1, 1);

    QPushButton *border_color = new QPushButton("Change border color");
    connect(border_color, &QPushButton::clicked, this, [&] {
        auto items = scene->selectedDiagramItems();
        if(items.size() == 0) {
            return;
        }

        QColor cl = QColorDialog::getColor(items[0]->Pen().color(), this, "Choose color");
        if (cl.isValid()) {
            for(auto item : items) {
                QPen tmp = item->Pen();
                tmp.setColor(cl);
                item->setPen(tmp);
            }
        }
    });

    borderSpin = new QSpinBox;
    borderSpin->setRange(1, 100);
    borderSpin->setMaximumWidth(100);
    connect(borderSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        for(auto i : scene->selectedDiagramItems()) {
            QPen tmp = dynamic_cast<DiagramItem *>(i)->Pen();
            tmp.setWidth(val);
            dynamic_cast<DiagramItem *>(i)->setPen(tmp);
        }
    });

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(new QLabel("Border width"));
    hl->addWidget(borderSpin);

    vl->addWidget(item_color);
    vl->addItem(item_settings);
    vl->addWidget(border_color);
    vl->addItem(hl);
    vl->addStretch();

    QWidget *tab = new QWidget;
    tab->setLayout(vl);
    return tab;
}

QWidget *MainWindow::createArrowTab()
{
    QPushButton *arrow_color = new QPushButton("Change line color");
    connect(arrow_color, &QPushButton::clicked, this, [&] {
        auto arrows = scene->selectedArrows();
        if(arrows.size() == 0) {
            return;
        }

        QColor cl = QColorDialog::getColor(arrows[0]->Pen().color(), this, "Choose color");
        if (cl.isValid()) {
            for(auto arrow : arrows) {
                QPen tmp = arrow->Pen();
                tmp.setColor(cl);
                arrow->setPen(tmp);
            }
        }
    });

    arrowSpin = new QSpinBox;
    arrowSpin->setRange(1, 100);
    arrowSpin->setMaximumWidth(100);
    connect(arrowSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        foreach(auto i, scene->selectedArrows()) {
            QPen tmp = i->Pen();
            tmp.setWidth(val);
            i->setPen(tmp);
        }
    });

    arrowheadSpin = new QSpinBox;
    arrowheadSpin->setRange(1, 500);
    arrowheadSpin->setMaximumWidth(100);
    connect(arrowheadSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int val) {
        foreach(auto i, scene->selectedArrows()) {
            i->setArrowheadLength(val);
        }
    });

    connect(scene, &DiagramScene::selectionChanged, this, [&] {
        auto items = scene->selectedArrows();
        if(items.size() == 1) {
            arrowSpin->setValue(items[0]->Pen().width());
            arrowheadSpin->setValue(items[0]->arrowheadLength());
            arrowheadCombo->setCurrentIndex(items[0]->arrowhead());
        }
    });

    QHBoxLayout *hl1 = new QHBoxLayout;
    hl1->addWidget(new QLabel("Arrow width"));
    hl1->addWidget(arrowSpin);

    QHBoxLayout *hl2 = new QHBoxLayout;
    hl2->addWidget(new QLabel("Arrowhead length"));
    hl2->addWidget(arrowheadSpin);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(arrow_color);
    vl->addItem(hl1);
    vl->addItem(hl2);
    vl->addStretch();

    QWidget *tab = new QWidget;
    tab->setLayout(vl);
    return tab;
}

void MainWindow::setComboCurScale()
{
    sceneScaleCombo->setCurrentText(QString::number(
                                        view->transform().m11()*100, 'f', 1 )+"%");
}

int MainWindow::askForSave()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Redactor");
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

void MainWindow::handleReadWriteErrors(IReaderWriter::Status st)
{
    switch (st) {
    case IReaderWriter::Succes: {
        sceneIsSaved = false;
        return;
    }
    case IReaderWriter::OpenError: {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Can't open the file!");
//        messageBox.setFixedSize(500,200);
    }
    case IReaderWriter::ReadError: {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Something went wrong while reading file.");
    }
    case IReaderWriter::WriteError: {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Something went wrong while writing into file.");
    }
    }
}

void MainWindow::updateScene()
{
    DiagramScene *new_scene = new DiagramScene;
    view->setScene(new_scene);
    if(scene){
        scene->deleteLater();
    }
    scene = new_scene;
    view->centerOn(scene->paper);

    auto button_id = actionTypeGroup->checkedId();
    if(button_id != -1) {
        scene->setState(static_cast<DiagramScene::Mode>(button_id));
    }

    button_id = buttonGroup->checkedId();
    if(button_id != -1) {
        scene->setState(DiagramScene::AddItem);
//        scene->setItemTypeFlags(button_id);
        scene->setItemType(static_cast<DiagramItem::ItemType>(button_id / 1000));
        scene->setItemSubtype(button_id % 1000);
    }
    connect(scene, &DiagramScene::selectionChanged, this, [&] {
        auto items = scene->selectedDiagramItems();
        if(items.size() == 1) {
            widthSpin->setValue(round(items[0]->Size().width()));
            heightSpin->setValue(round(items[0]->Size().height()));
            borderSpin->setValue(items[0]->Pen().width());
            QFont font(items[0]->font());
            qDebug() << font;
            fontCombo->setFont(font.family());
            fontSizeSpin->setValue(font.pixelSize());
            boldAction->setChecked(font.bold());
            italicAction->setChecked(font.italic());
            underlineAction->setChecked(font.underline());
            itemTextEdit->setPlainText(items[0]->Text());
        }
    });
    connect(scene, &DiagramScene::changed, this, [&] (const QList<QRectF> &list) {
        if(list.size() != 0 && !(list.size() == 1 && list[0] == scene->sceneRect())) {
            sceneIsSaved = false;
        }
    });
}

QToolButton *MainWindow::createToolBoxButton(DiagramItem &item, int id)
{
    QIcon icon(item.icon());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, id);

    return button;
}

QComboBox *MainWindow::createArrowheadsCombo()
{
    QComboBox *combo = new QComboBox();
    for(int i = Arrow::begin; i < Arrow::end; i++) {
        Arrow a(QLine(0, 0, 0, 0));
        combo->addItem(a.arrowheadIcon(static_cast<Arrow::ArrowheadType>(i)), QString::number(i),
                            static_cast<Arrow::ArrowheadType>(i));
    }
    return combo;
}

void MainWindow::opentPreviewDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    printer.setPageSize((QPrinter::PageSize)scene->paper->size());
    printer.setOrientation((QPrinter::Orientation)scene->paper->orientation());

    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [&](QPrinter *previewPrinter) {
        QPainter p(previewPrinter);
        QRectF old_rect = scene->sceneRect();
        scene->setSceneRect(scene->paper->rect());
        scene->clearSelection();
        scene->render(&p);
        scene->setSceneRect(old_rect);
        view->centerOn(scene->paper);
        p.end();
    });

    dialog->exec();

    delete dialog;
}
