#include "sources/soundsource.h"

#include <QMimeDatabase>
#include <QMimeType>

#include "sources/soundsourceproxy.h"
#include "util/logger.h"

namespace mixxx {

namespace {

const Logger kLogger("SoundSource");

inline QUrl validateLocalFileUrl(QUrl url) {
    DEBUG_ASSERT(url.isValid());
    VERIFY_OR_DEBUG_ASSERT(url.isLocalFile()) {
        kLogger.warning()
                << "Unsupported URL:"
                << url.toString();
    }
    return url;
}

} // anonymous namespace

//static
QString SoundSource::getTypeFromUrl(const QUrl& url) {
    const QString filePath = validateLocalFileUrl(url).toLocalFile();
    return getTypeFromFile(QFileInfo(filePath));
}

//static
QString SoundSource::getTypeFromFile(const QFileInfo& fileInfo) {
    const QString fileSuffix = fileInfo.suffix();

    if (fileSuffix == "opus") {
        // opus has mime type "audio/ogg" which will be decoded with the SoundSourceOggVobis()
        // but we want SoundSourceOpus()
        return fileSuffix;
    }

    const QMimeType mimeType = QMimeDatabase().mimeTypeForFile(
            fileInfo, QMimeDatabase::MatchContent);
    // According to the documentation mimeTypeForFile always returns a valid
    // type, using the generic type application/octet-stream as a fallback.
    // This might also occur for missing files as seen on Qt 5.12.
    if (!mimeType.isValid() || mimeType.isDefault()) {
        qInfo() << "Unable to detect MIME type for file" << fileInfo.filePath();
        return fileSuffix;
    }
    const QString fileType = SoundSourceProxy::getFileTypeByMimeType(mimeType);
    if (fileType.isEmpty()) {
        qWarning() << "No file type registered for MIME type" << mimeType;
        return fileSuffix;
    }
    if (fileType != fileSuffix && mimeType.suffixes().contains(fileSuffix)) {
        qWarning()
                << "Using type" << fileType
                << "instead of" << fileSuffix
                << "according to the detected MIME type" << mimeType
                << "of file" << fileInfo.filePath();
    }
    return fileType;
}

SoundSource::SoundSource(const QUrl& url, const QString& type)
        : AudioSource(validateLocalFileUrl(url)),
          MetadataSourceTagLib(getLocalFileName()),
          m_type(type) {
}

} // namespace mixxx
