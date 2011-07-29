#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QVector>

#include "waveformrendererabstract.h"
#include "trackinfoobject.h"

#include <QDebug>

class QTime;
class QPainter;
class TrackInfoObject;
class WaveformRendererAbstract;
class ControlObjectThreadMain;

class WaveformWidgetRenderer
{
public:
    WaveformWidgetRenderer( const char* group);
    virtual ~WaveformWidgetRenderer();

    void init();
    void setup( const QDomNode& node);
    void preRender();
    void draw( QPainter* painter, QPaintEvent* event);

    const char* getGroup() const { return m_group;}
    const TrackPointer getTrackInfo() const { return m_trackInfoObject;}

    double getFirstDisplayedPosition() const { return m_firstDisplayedPosition;}
    double getLastDisplayedPosition() const { return m_lastDisplayedPosition;}

    bool zoomIn();
    bool zoomOut();

    void updateSamplingPerPixel();
    double getVisualSamplePerPixel();
    double getAudioSamplePerPixel();

    //those function replace at its best sample position to an admissible
    //sample position according to the current visual resampling
    //this make mark and signal deterministic
    void regulateVisualSample( int& sampleIndex);
    void regulateAudioSample( int& sampleIndex);

    double getPlayPos() const { return m_playPos;}
    double getZoomFactor() const { m_zoomFactor;}
    double getRateAdjust() const { return m_rateAdjust;}
    double getGain() const { return m_gain;}
    int getTrackSamples() const { return m_trackSamples;}

    void resize( int width, int height);
    int getHeight() const { return m_height;}
    int getWidth() const { return m_width;}

    template< class T_Renderer>
    inline void addRenderer() { m_rendererStack.push_back( new T_Renderer(this));}

    void setTrack( TrackPointer track);

protected:
    const char* m_group;
    TrackPointer m_trackInfoObject;
    QVector<WaveformRendererAbstract*> m_rendererStack;
    int m_height;
    int m_width;

    double m_firstDisplayedPosition;
    double m_lastDisplayedPosition;

    double m_rateAdjust;
    double m_zoomFactor;
    double m_visualSamplePerPixel;
    double m_audioSamplePerPixel;

    //TODO vRince create some class to manage control/value
    //ControlConnection
    ControlObject* m_playPosControlObject;
    double m_playPos;
    ControlObject* m_rateControlObject;
    double m_rate;
    ControlObject* m_rateRangeControlObject;
    double m_rateRange;
    ControlObject* m_rateDirControlObject;
    double m_rateDir;
    ControlObject* m_gainControlObject;
    double m_gain;
    ControlObject* m_trackSamplesControlObject;
    int m_trackSamples;

    //Debug
    QTime* m_timer;
    int m_lastFrameTime;
    int m_lastFramesTime[100];
    int m_lastSystemFrameTime;
    int m_lastSystemFramesTime[100];
    int currentFrame;

};

#endif // WAVEFORMWIDGET_H
