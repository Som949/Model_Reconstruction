#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QDir>


class QListWidget;
class QStackedWidget;
class QPushButton;
class QComboBox;
class QListWidgetItem;

static const QString defaultProjectPath = QDir::homePath() + "/Voxel-Forge/";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Menu actions
    void openProjectFolder();
    void showAbout();

    // Buttons / UI
    void launchColmap();
    void changePage(int index);
    void setTheme(int index);

    // Image manager
    void addImages();
    void saveSelectedImages();
    void deleteSelectedImages();
    void createNewFolder();
    void changeFolder(const QString &folderName);

private:
    void createMenuBar();
    QWidget* createHomePage();
    QWidget* createProjectManagerPage();
    QWidget* createImageManagerPage();
    QWidget* createSettingsPage();

    // UI members
    QListWidget *sidebar = nullptr;
    QStackedWidget *stackedContent = nullptr;

    // Image manager widgets
    QListWidget *imageList = nullptr;
    QPushButton *addImageButton = nullptr;
    QPushButton *saveImagesButton = nullptr;
    QPushButton *deleteImagesButton = nullptr;   // new button

    // theme
    QComboBox *themeCombo = nullptr;

    // state
    QString currentProjectFolder = defaultProjectPath;
    // Folder
    QString currentImageFolder;
    QPushButton *newFolderButton = nullptr;
};

#endif // MAINWINDOW_H
