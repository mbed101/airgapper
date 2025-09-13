#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <qrencode.h>

class QRCodeWidget : public QWidget
{
    Q_OBJECT

public:
    QRCodeWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        setupUI();
        generateQRCode("Hello, World!");
    }

private slots:
    void onGenerateClicked()
    {
        QString text = textInput->text();
        if (!text.isEmpty()) {
            generateQRCode(text);
        }
    }

private:
    void setupUI()
    {
        setWindowTitle("QR Code Generator");
        setFixedSize(400, 500);

        auto *layout = new QVBoxLayout(this);
        
        // Input section
        auto *inputLayout = new QHBoxLayout();
        textInput = new QLineEdit();
        textInput->setPlaceholderText("Enter text to generate QR code");
        textInput->setText("Hello, World!");
        
        auto *generateButton = new QPushButton("Generate QR Code");
        connect(generateButton, &QPushButton::clicked, this, &QRCodeWidget::onGenerateClicked);
        
        inputLayout->addWidget(textInput);
        inputLayout->addWidget(generateButton);
        
        // QR Code display
        qrLabel = new QLabel();
        qrLabel->setAlignment(Qt::AlignCenter);
        qrLabel->setMinimumSize(300, 300);
        qrLabel->setStyleSheet("border: 1px solid gray;");
        
        layout->addLayout(inputLayout);
        layout->addWidget(qrLabel);
        layout->addStretch();
    }
    
    void generateQRCode(const QString &text)
    {
        QRcode *qr = QRcode_encodeString(text.toUtf8().constData(), 
                                        0, QR_ECLEVEL_L, QR_MODE_8, 1);
        
        if (!qr) {
            QMessageBox::warning(this, "Error", "Failed to generate QR code");
            return;
        }
        
        // Create QPixmap from QR code data
        int qrSize = qr->width;
        int scale = 8; // Scale factor to make QR code bigger
        
        QPixmap pixmap(qrSize * scale, qrSize * scale);
        pixmap.fill(Qt::white);
        
        QPainter painter(&pixmap);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        
        for (int y = 0; y < qrSize; ++y) {
            for (int x = 0; x < qrSize; ++x) {
                if (qr->data[y * qrSize + x] & 1) {
                    painter.drawRect(x * scale, y * scale, scale, scale);
                }
            }
        }
        
        qrLabel->setPixmap(pixmap.scaled(qrLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        QRcode_free(qr);
    }
    
    QLineEdit *textInput;
    QLabel *qrLabel;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QRCodeWidget widget;
    widget.show();
    
    return app.exec();
}

#include "main.moc"