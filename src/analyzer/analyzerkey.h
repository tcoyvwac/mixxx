#pragma once

#include <QHash>
#include <QList>
#include <QString>

#include "analyzer/analyzer.h"
#include "analyzer/plugins/analyzerplugin.h"
#include "analyzer/plugins/analyzerpluginsupport.h"
#include "preferences/keydetectionsettings.h"
#include "preferences/usersettings.h"
#include "track/track_decl.h"
#include "util/memory.h"

class AnalyzerKey : protected AnalyzerPluginSupportInfo, public Analyzer {
  public:
    explicit AnalyzerKey(const KeyDetectionSettings& keySettings);
    ~AnalyzerKey() override = default;

    static QList<mixxx::AnalyzerPluginInfo> availablePlugins();

    bool initialize(TrackPointer tio,
            mixxx::audio::SampleRate sampleRate,
            int totalSamples) override;
    bool processSamples(const CSAMPLE *pIn, const int iLen) override;
    void storeResults(TrackPointer tio) override;
    void cleanup() override;

  private:
    bool shouldAnalyze(TrackPointer tio) const;

    KeyDetectionSettings m_keySettings;
    std::unique_ptr<mixxx::AnalyzerKeyPlugin> m_pPlugin;
    int m_iSampleRate;
    int m_iTotalSamples;
    int m_iMaxSamplesToProcess;
    int m_iCurrentSample;

    bool m_bPreferencesKeyDetectionEnabled;
    bool m_bPreferencesFastAnalysisEnabled;
    bool m_bPreferencesReanalyzeEnabled;
};
