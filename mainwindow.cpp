#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QProcess>
#include <QImageReader>
#include <QPixmap>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QApplication> // for qApp, setStyle, setStyleSheet
#include <QInputDialog>
#include <QLineEdit>
#include <QFrame>
#include "cubewidget.h"
#include <QStyleFactory>
#include <QDir>
#include <QApplication>   // for qApp, setStyle, setStyleSheet
#include <QGraphicsDropShadowEffect> // Ensure this is at the top of your mainwindow.cpp
#include <QDebug> // Add this for debugging image loading issues
#include <QPainter> // Add this include for QPainter
#include <QBitmap> // Add this include for QBitmap


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Voxel Forge");

    // central widget layout: left sidebar + right stacked content
    QWidget *central = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(16);
    this->setStyleSheet(
        "QMainWindow { background-color: #030312; }" // A deep dark purple/black color
        );

    // Sidebar (left)
    sidebar = new QListWidget;
    sidebar->setFixedWidth(200);
    sidebar->setSpacing(8);
    sidebar->setFocusPolicy(Qt::NoFocus);
    sidebar->addItem("Home");
    sidebar->addItem("Project Manager");
    sidebar->addItem("Image Manager");
    sidebar->addItem("Settings");
    sidebar->setCurrentRow(0);

    sidebar->setStyleSheet(
        "QListWidget { "
        "background: rgba(0,0,0,0.2); "
        "color: #eaeaea; "
        "font-size: 14px; "
        "border-radius: 8px; "
        "outline: none; "
        "}"
        "QListWidget::item { "
        "padding: 12px 16px; "
        "margin: 4px 8px; "
        "border-radius: 6px; "
        "outline: none; "
        "border: none; "
        "}"
        "QListWidget::item:selected { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7b61ff, stop:1 #9b7dff); "
        "color: white; "
        "font-weight: 600; "
        "outline: none; "
        "border: none; "
        "}"
        "QListWidget::item:hover:!selected { "
        "background: rgba(255,255,255,0.05); "
        "}"
        "QListWidget::item:focus { "
        "outline: none; "
        "border: none; "
        "}"
        );
    mainLayout->addWidget(sidebar);

    // Stacked content (right)
    stackedContent = new QStackedWidget;
    stackedContent->addWidget(createHomePage());
    stackedContent->addWidget(createProjectManagerPage());
    stackedContent->addWidget(createImageManagerPage());
    stackedContent->addWidget(createSettingsPage());
    mainLayout->addWidget(stackedContent, 1);

    setCentralWidget(central);

    createMenuBar();

    // connect
    connect(sidebar, &QListWidget::currentRowChanged, this, &MainWindow::changePage);

    // default style/theme
    setTheme(0);
}


void MainWindow::createMenuBar()
{
    QMenuBar *mb = menuBar();
    mb->setNativeMenuBar(false); // keep menu bar inside window on Windows/Linux

    QMenu *file = mb->addMenu("File");
    QAction *openAct = file->addAction("Open Project Folder...");
    connect(openAct, &QAction::triggered, this, &MainWindow::openProjectFolder);
    file->addSeparator();
    QAction *exitAct = file->addAction("Exit");
    connect(exitAct, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *tools = mb->addMenu("Tools");
    QAction *colmapAct = tools->addAction("Launch COLMAP GUI");
    connect(colmapAct, &QAction::triggered, this, &MainWindow::launchColmap);

    QMenu *help = mb->addMenu("Help");
    QAction *aboutAct = help->addAction("About");
    connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);

    // subtle styling for menu bar
    mb->setStyleSheet("QMenuBar { background: transparent; color: #eaeaea; } QMenuBar::item { spacing: 6px; padding: 4px 12px; }");
}

QWidget* MainWindow::createHomePage()
{
    QWidget *w = new QWidget;
    // Use a vertical layout for the content
    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->setSpacing(26); // Default spacing between widgets

    // --- 1. Title (Centered) ---
    QLabel *title = new QLabel("Welcome to Voxel Forge");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 52px; color: #ffffff; font-weight: 700;");
    contentLayout->addWidget(title);
    contentLayout->addSpacing(30); // Space after title

    // --- 2. Cube Widget ---
    CubeWidget *cube = new CubeWidget();
    cube->setFixedSize(180, 180); // Size of the cube widget
    contentLayout->addWidget(cube, 0, Qt::AlignCenter);
    contentLayout->addSpacing(40); // Increased space after cube

    // --- 3. Features Section Title ---
    QLabel *featuresTitle = new QLabel("Key Features");
    featuresTitle->setAlignment(Qt::AlignCenter);
    featuresTitle->setStyleSheet("font-size: 20px; color: #ffffff; font-weight: 600;");
    contentLayout->addWidget(featuresTitle);
    contentLayout->addSpacing(15); // Space between feature title and list

    // --- 4. Feature List ---
    QString featuresText =
        "• Create detailed 3D models from multiple images\n"
        "• Generate Sparse and Dense Point Clouds\n"
        "• View and explore 3D reconstructions in VR\n"
        "• Manage project images easily\n"
        "• Intuitive workflow and modern UI";

    QLabel *featuresList = new QLabel(featuresText);
    featuresList->setAlignment(Qt::AlignCenter);
    featuresList->setWordWrap(true);
    featuresList->setStyleSheet("font-size: 16px; color: #cfcfcf; line-height: 1.4;");
    contentLayout->addWidget(featuresList);
    contentLayout->addSpacing(40); // Space before divider

    // --- 5. Divider Line ---
    QFrame *divider = new QFrame;
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Plain); // Changed from Sunken
    divider->setLineWidth(0); // Ensure no line width
    divider->setFixedHeight(1); // Explicitly set height to 1px
    divider->setMinimumWidth(300); // Fixed width
    divider->setMaximumWidth(300);
    // Darker, subtle grey line, using a solid background-color for consistent thickness
    divider->setStyleSheet("background-color: rgba(255,255,255,0.1); border: none;");
    contentLayout->addWidget(divider, 0, Qt::AlignCenter);
    contentLayout->addSpacing(30); // Space after divider

    // --- Container for Content Layout ---
    QWidget *contentWidget = new QWidget;
    contentWidget->setLayout(contentLayout);

    // --- Main Layout for Centering ---
    QVBoxLayout *mainLayout = new QVBoxLayout(w);
    mainLayout->addStretch();           // Push content down
    mainLayout->addWidget(contentWidget, 0, Qt::AlignCenter); // Center the content widget horizontally
    mainLayout->addStretch();           // Push content up

    // Add margins to prevent the content from touching the edges of the stacked widget
    mainLayout->setContentsMargins(50, 50, 50, 50);

    return w;
}

QWidget* MainWindow::createProjectManagerPage()
{
    QWidget *w = new QWidget;
    QVBoxLayout *mainPageLayout = new QVBoxLayout(w);
    mainPageLayout->setContentsMargins(25, 25, 25, 25);

    QLabel *title = new QLabel("Project Manager");
    title->setAlignment(Qt::AlignLeft);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #ffffff; margin-bottom: 20px;");
    mainPageLayout->addWidget(title);

    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(25);

    auto makeProjectCard = [&](const QString &text, const QString &imagePath) {
        QWidget *card = new QWidget();
        card->setMinimumSize(220, 250);
        card->setCursor(Qt::PointingHandCursor);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(15, 15, 15, 15);
        cardLayout->setSpacing(15);
        cardLayout->setAlignment(Qt::AlignCenter);

        // --- 1. Images ke corners ko rounded karna ---
        // Image display ke liye ek naya QWidget container banayenge
        // Taki hum us container par style apply kar sakein
        QWidget *imageContainer = new QWidget();
        imageContainer->setFixedSize(250, 250); // Image size ke barabar container size

        QLabel *imageLabel = new QLabel(imageContainer); // QLabel ko container ke andar rakha
        imageLabel->setGeometry(0, 0, 250, 250); // Label ka size container ke barabar
        imageLabel->setAlignment(Qt::AlignCenter);

        QPixmap originalPixmap(imagePath);
        if (originalPixmap.isNull()) {
            qDebug() << "Failed to load image:" << imagePath;
            imageLabel->setText("No Image");
            imageLabel->setStyleSheet("color: #aaaaaa; font-size: 14px;");
        } else {
            // Pixmap ko round karne ke liye mask apply karna
            QPixmap scaledPixmap = originalPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPixmap roundedPixmap(scaledPixmap.size());
            roundedPixmap.fill(Qt::transparent); // Transparent background

            QPainter painter(&roundedPixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);
            painter.setBrush(QBrush(scaledPixmap));
            painter.setPen(Qt::NoPen);

            // Corner radius jitna aap chahte hain
            int radius = 15; // Image ke corners ka radius
            painter.drawRoundedRect(roundedPixmap.rect(), radius, radius);

            imageLabel->setPixmap(roundedPixmap);
            imageLabel->setStyleSheet("background: transparent; border: none;");
        }

        cardLayout->addWidget(imageContainer); // Container ko layout me add kiya

        cardLayout->addStretch();

        // --- 2. Aur 3. Neeche wale "button" (ab QWidget container) ke corners rounded karna aur look better karna ---
        // Text ko wrap karne ke liye ek QWidget container banayenge
        // Us container ko button jaisa style denge
        QWidget *textButtonContainer = new QWidget();
        // textButtonContainer->setMinimumHeight(40); // Minimum height set kar sakte hain
        textButtonContainer->setStyleSheet(
            "QWidget {"
            "  background-color: #6c63ff; " // Button ka background color
            "  border-radius: 8px; "         // Button ke corners rounded
            "  padding: 8px 12px; "          // Padding for text inside
            "}"
            "QWidget:hover {"
            "  background-color: #7f75ff; " // Hover color
            "}"
            );
        textButtonContainer->setCursor(Qt::PointingHandCursor); // Cursor change for "button" effect

        // Text ke liye QLabel ko is container ke andar rakha
        QLabel *textLabel = new QLabel(text, textButtonContainer);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setWordWrap(true);
        textLabel->setStyleSheet("color: white; font-size: 15px; font-weight: 500; background: transparent;"); // Text ka color white, background transparent

        // QLabel ko container ke andar adjust karna
        QHBoxLayout *textLayout = new QHBoxLayout(textButtonContainer);
        textLayout->addWidget(textLabel);
        textLayout->setContentsMargins(0,0,0,0); // No extra margins

        cardLayout->addWidget(textButtonContainer); // Container ko main card layout me add kiya
        cardLayout->addStretch(); // Text button ke neeche space

        // Card ki styling (no change here, this was fine)
        card->setStyleSheet(
            "QWidget { "
            "  background-color: #030317; "
            "  border: 1px solid #3c3c3c; "
            "  border-radius: 12px; "
            "}"
            "QWidget:hover { "
            "  background-color: #02021c; "
            "  border: 1px solid #7b61ff; "
            "}"
            );

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(20);
        shadow->setXOffset(0);
        shadow->setYOffset(5);
        shadow->setColor(QColor(0, 0, 0, 100));
        card->setGraphicsEffect(shadow);

        return card;
    };

    cardsLayout->addWidget(makeProjectCard("Generate Sparse Cloud", ":/icons/icons/sparse.jpg"));
    cardsLayout->addWidget(makeProjectCard("Generate Dense Cloud", ":/icons/icons/dense.jpg"));
    cardsLayout->addWidget(makeProjectCard("View Constructed 3D Model", ":/icons/icons/model.jpg"));
    cardsLayout->addWidget(makeProjectCard("VR Connect", ":/icons/icons/vr.jpg"));

    mainPageLayout->addLayout(cardsLayout);
    mainPageLayout->addStretch();

    return w;
}

QWidget* MainWindow::createImageManagerPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *outer = new QVBoxLayout(page);
    outer->setSpacing(12);

    // Header row with title and action buttons
    QHBoxLayout *header = new QHBoxLayout;
    QLabel *title = new QLabel("Image Manager");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");
    header->addWidget(title);
    header->addStretch();

    addImageButton = new QPushButton("+ Add Images");
    addImageButton->setCursor(Qt::PointingHandCursor);
    addImageButton->setFixedHeight(36);

    addImageButton->setStyleSheet(
        "QPushButton { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #6c63ff, stop:1 #8b7fff); "
        "color: white; "
        "border-radius: 8px; "
        "padding: 6px 16px; "
        "font-weight: 600; "
        "border: 1px solid rgba(0,0,0,0.1); "
        "} "
        "QPushButton:hover { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7f75ff, stop:1 #9f8fff); "
        "border-color: rgba(0,0,0,0.2); "
        "box-shadow: 0 0 12px rgba(140,130,255,0.6); "
        "} "
        "QPushButton:pressed { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #5a50e0, stop:1 #7260f5); "
        "} "
        );


    saveImagesButton = new QPushButton("Save Selected...");
    saveImagesButton->setCursor(Qt::PointingHandCursor);
    saveImagesButton->setFixedHeight(36);

    saveImagesButton->setStyleSheet(
        "QPushButton { "
        "background-color: #2ecc71; "
        "color: white; "
        "border-radius: 8px; "
        "padding: 6px 16px; "
        "border: 1px solid #27ae60; "
        "font-weight: 500; "
        "}"
        "QPushButton:hover { "
        "background-color: #27ae60; "
        "border-color: #1e8449; "
        "}"
        );

    deleteImagesButton = new QPushButton("Delete Selected");
    deleteImagesButton->setCursor(Qt::PointingHandCursor);
    deleteImagesButton->setFixedHeight(36);

    deleteImagesButton->setStyleSheet(
        "QPushButton { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #dc3545, stop:1 #c82333); "
        "color: white; "
        "border-radius: 8px; "
        "padding: 6px 16px; "
        "font-weight: 600; "
        "border: 1px solid rgba(0,0,0,0.1); "
        "transition: all 0.2s ease-in-out; "
        "} "
        "QPushButton:hover { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #e74c3c, stop:1 #d62c1a); "
        "border-color: rgba(0,0,0,0.2); "
        "box-shadow: 0 0 12px rgba(220,53,69,0.5); "
        "} "
        "QPushButton:pressed { "
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #bd2130, stop:1 #a71d2a); "
        "} "
        );


    header->addWidget(deleteImagesButton);  // add to header row
    header->addWidget(addImageButton);
    header->addWidget(saveImagesButton);
    outer->addLayout(header);

    // Image list as icon grid
    imageList = new QListWidget;
    imageList->setViewMode(QListView::IconMode);
    imageList->setIconSize(QSize(160, 120));
    imageList->setResizeMode(QListWidget::Adjust);
    imageList->setMovement(QListView::Static);
    imageList->setSpacing(12);
    imageList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    imageList->setStyleSheet(
        "QListWidget { background: rgba(0,0,0,0.15); border: 1px solid rgba(255,255,255,0.05); "
        "border-radius: 8px; padding: 12px; }"
        "QListWidget::item { color: #eaeaea; font-size: 12px; background: rgba(255,255,255,0.03); "
        "border-radius: 6px; padding: 8px; }"
        "QListWidget::item:selected { background: rgba(123,97,255,0.2); border: 2px solid #7b61ff; }"
        "QListWidget::item:hover { background: rgba(255,255,255,0.08); }"
        );

    outer->addWidget(imageList, 1);

    // connections
    connect(addImageButton, &QPushButton::clicked, this, &MainWindow::addImages);
    connect(saveImagesButton, &QPushButton::clicked, this, &MainWindow::saveSelectedImages);
    connect(deleteImagesButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedImages);

    return page;
}

QWidget* MainWindow::createSettingsPage()
{
    QWidget *w = new QWidget;
    QVBoxLayout *v = new QVBoxLayout(w);

    QLabel *title = new QLabel("Settings");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #ffffff;");
    v->addWidget(title);

    QHBoxLayout *themeRow = new QHBoxLayout;
    QLabel *themeLabel = new QLabel("Theme:");
    themeLabel->setStyleSheet("color: #cfcfcf;");
    themeCombo = new QComboBox;
    themeCombo->addItem("Dark (Default)");
    themeCombo->addItem("Fusion (Light)");
    themeCombo->setFixedWidth(180);
    themeCombo->setStyleSheet("QComboBox { padding: 6px; }");

    themeRow->addWidget(themeLabel);
    themeRow->addWidget(themeCombo);
    themeRow->addStretch();
    v->addLayout(themeRow);
    v->addStretch();

    connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::setTheme);

    return w;
}

// Slots

void MainWindow::openProjectFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Project Folder", currentProjectFolder);
    if (!dir.isEmpty()) {
        currentProjectFolder = dir;
        QMessageBox::information(this, "Project Folder", QString("Project folder set to:\n%1").arg(currentProjectFolder));
    }
}

void MainWindow::showAbout()
{
    QString aboutText =
        "<h3>Voxel Forge</h3>"
        "<p><b>Version:</b> 1.0.0</p>"
        "<p>A 3D Model Reconstruction and Interaction System built with Qt.</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Import and manage project images</li>"
        "<li>Reconstruct sparse and dense point clouds</li>"
        "<li>View and interact with 3D models</li>"
        "<li>Connect with VR systems</li>"
        "</ul>"
        "<p><b>Developed by:</b> Darshan, Tanishq, Sanskar, Prayag and Maharishi</p>"
        "<p><b>Built with:</b> Qt, COLMAP, Unreal and other open-source libraries.</p>"
        "<p>&copy; 2025 All Rights Reserved.</p>";

    QMessageBox::about(this, "About Voxel Forge", aboutText);
}



void MainWindow::createNewFolder()
{
    if (currentProjectFolder.isEmpty()) {
        QMessageBox::warning(this, "No Project", "Please open a project folder first.");
        return;
    }

    QString folderName = QInputDialog::getText(this, "New Folder", "Enter folder name:");
    if (folderName.isEmpty()) return;

    QDir dir(currentProjectFolder);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Error", "Project folder path is invalid.");
        return;
    }

    QString newFolderPath = dir.filePath(folderName);
    if (QDir(newFolderPath).exists()) {
        QMessageBox::warning(this, "Error", "Folder already exists.");
        return;
    }

    if (!dir.mkdir(folderName)) {
        QMessageBox::warning(this, "Error", "Failed to create folder.");
    } else {
        QMessageBox::information(this, "Folder Created", "Folder created successfully!");

        // ✅ Refresh Image Manager to show new folder
        changeFolder(currentProjectFolder);


    }

}



void MainWindow::launchColmap()
{
    // Try start colmap gui (if in PATH)
    QProcess::startDetached("colmap", QStringList() << "gui");
}

void MainWindow::changePage(int index)
{
    stackedContent->setCurrentIndex(index);
}

void MainWindow::setTheme(int index)
{
    if (index == 0) {
        // Dark theme
        qApp->setStyleSheet(
            "QMainWindow { background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #141418, stop:1 #1e1e24); }"
            "QLabel { color: #eaeaea; }"
            "QMenuBar { color: #eaeaea; }"
            );
        qApp->setStyle(QStyleFactory::create("Fusion"));
    } else {
        qApp->setStyleSheet("");
        qApp->setStyle(QStyleFactory::create("Fusion"));
    }
}

// Image manager: add images (open explorer and display thumbnails)
void MainWindow::addImages()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this, "Select Images", QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.tiff)");

    if (files.isEmpty()) return;

    QString savePath = currentProjectFolder;

    QDir dir(savePath);
    if (!dir.exists()) dir.mkpath(".");

    for (const QString &f : files) {
        QFileInfo fi(f);
        QPixmap pm(f);
        if (pm.isNull()) continue;

        // ✅ Save into project folder
        QString dest = dir.filePath(fi.fileName());
        QFile::copy(f, dest);

        // ✅ Create thumbnail
        QPixmap scaled = pm.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // ✅ Add to imageList
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(scaled));
        item->setText(fi.fileName());
        item->setToolTip(dest); // show saved path on hover
        item->setData(Qt::UserRole, dest); // store full path
        imageList->addItem(item);       // <-- missing line before
    }
}


// Save selected images to a folder (copies)
void MainWindow::saveSelectedImages()
{
    QList<QListWidgetItem*> selected = imageList->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "No selection", "Please select one or more images from the list to save.");
        return;
    }

    // default destination suggestion: currentProjectFolder (create if missing)
    QDir dir(currentProjectFolder);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString dest = QFileDialog::getExistingDirectory(this, "Select Destination Folder", currentProjectFolder);
    if (dest.isEmpty()) return;

    int copied = 0;
    QStringList failed;
    for (QListWidgetItem *it : selected) {
        QString src = it->data(Qt::UserRole).toString();
        if (src.isEmpty()) continue;
        QFileInfo fi(src);
        QString dstPath = QDir(dest).filePath(fi.fileName());

        // if exists, create unique name
        int idx = 1;
        QString base = fi.completeBaseName();
        QString ext = fi.suffix();
        while (QFile::exists(dstPath)) {
            QString newName = QString("%1_%2.%3").arg(base).arg(idx).arg(ext);
            dstPath = QDir(dest).filePath(newName);
            idx++;
        }

        if (QFile::copy(src, dstPath)) {
            copied++;
        } else {
            failed << src;
        }
    }

    QString msg = QString("Copied %1 file(s) to:\n%2").arg(copied).arg(dest);
    if (!failed.isEmpty()) {
        msg += QString("\n\nFailed to copy %1 file(s).").arg(failed.count());
    }
    QMessageBox::information(this, "Save Complete", msg);
}
void MainWindow::changeFolder(const QString &path)
{
    // Update the current image folder
    currentImageFolder = path;

    // Clear existing list
    imageList->clear();

    // Populate images from the new folder
    QDir dir(path);
    QStringList filters = {"*.png", "*.jpg", "*.jpeg", "*.bmp", "*.tiff"};
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo &fi : files) {
        QPixmap pm(fi.absoluteFilePath());
        if (pm.isNull()) continue;

        QPixmap scaled = pm.scaled(imageList->iconSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(scaled));
        item->setText(fi.fileName());
        item->setToolTip(fi.absoluteFilePath());
        item->setData(Qt::UserRole, fi.absoluteFilePath());
        imageList->addItem(item);
    }
}


void MainWindow::deleteSelectedImages()
{
    QList<QListWidgetItem*> selected = imageList->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "No selection", "Please select one or more images to delete.");
        return;
    }

    if (QMessageBox::question(this, "Confirm Delete",
                              QString("Are you sure you want to remove %1 selected image(s) from the list?")
                                  .arg(selected.count())) != QMessageBox::Yes) {
        return;
    }

    for (QListWidgetItem *it : selected) {
        QString path = it->data(Qt::UserRole).toString();
        if (!path.isEmpty() && QFile::exists(path))
            QFile::remove(path);

        delete imageList->takeItem(imageList->row(it));
    }
}
