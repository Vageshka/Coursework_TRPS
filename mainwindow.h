#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QButtonGroup>
#include <QFontComboBox>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMenu>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>
#include <QToolBox>
#include <ireaderwriter.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void save();
    void openFile();
    void creatNew();
    void saveAs();
    void autosave();

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createToolBox();
    QWidget *createPaperTab();
    QWidget *createItemTab();
    QWidget *createArrowTab();
    void setComboCurScale();
    int askForSave();
    void handleReadWriteErrors(IReaderWriter::Status st);
    void updateScene();

    QToolButton *createToolBoxButton(DiagramItem &item, int id);
    QComboBox *createArrowheadsCombo();

    void opentPreviewDialog();

    QMenu *fileMenu;
    QMenu *exportMenu;
    QMenu *aboutMenu;

    QToolBar *scaleToolBar;
    QToolBar *defaultToolBar;
    QToolBar *textToolBar;

    QButtonGroup *actionTypeGroup;

    QAction *createAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *previewAction;
    QAction *exportPngAction;
    QAction *exportPdfAction;
    QAction *aboutAction;
    QAction *deleteAction;

    QComboBox *sceneScaleCombo;
    QComboBox *arrowheadCombo;

    QFontComboBox *fontCombo;
    QSpinBox *fontSizeSpin;
    QAction *boldAction;
    QAction *italicAction;
    QAction *underlineAction;
    QAction *textColorAction;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;

    QTabWidget *stylesTabs;
    QComboBox *paperSizeCombo;
    QComboBox *paperOrientationCombo;
    QTextEdit *itemTextEdit;
    QSpinBox *widthSpin;
    QSpinBox *heightSpin;
    QSpinBox *borderSpin;
    QSpinBox *arrowSpin;
    QSpinBox *arrowheadSpin;

    DiagramScene *scene;
    QGraphicsView *view;

    QString filename_;
    bool sceneIsSaved;

    QTimer *autosaveTimer;
};
#endif // MAINWINDOW_H
