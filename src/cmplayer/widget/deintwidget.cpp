#include "deintwidget.hpp"
#include "datacombobox.hpp"
#include "misc/record.hpp"
#include "video/deintcaps.hpp"

static constexpr auto GPU = DeintDevice::GPU;
static constexpr auto CPU = DeintDevice::CPU;
static constexpr auto OpenGL = DeintDevice::OpenGL;

struct DeintWidget::Data {
    bool updating = false, hwdec = false;
    DataComboBox *combo = nullptr;
    QCheckBox *gl = nullptr, *doubler = nullptr, *gpu = nullptr;
    QMap<DeintMethod, DeintCaps> caps;
    QList<DeintCaps> defaults;
    DecoderDevice decoder;
    auto current() -> DeintCaps&
        { return caps[(DeintMethod)combo->currentData().toInt()]; }
};

DeintWidget::DeintWidget(DecoderDevice decoder, QWidget *parent)
    : QWidget(parent)
    , d(new Data)
{
    d->decoder = decoder;
    d->hwdec = decoder == DecoderDevice::GPU;
    Record r("deint_caps");
    const auto name = DecoderDeviceInfo::name(decoder);
    const auto tokens = r.value(name).toStringList();
    for (const auto &token : tokens) {
        auto caps = DeintCaps::fromString(token);
        if (caps.isAvailable())
            d->caps[caps.method()] = caps;
    }
    d->combo = new DataComboBox(this);
    d->defaults = DeintCaps::list();
    for (auto &caps : d->defaults) {
        if (!caps.isAvailable())
            continue;
        if ((d->hwdec && !caps.hwdec()) || (!d->hwdec && !caps.swdec()))
            continue;
        const auto method = caps.method();
        d->combo->addItem(DeintMethodInfo::name(method), (int)method);
        if (!d->caps.contains(method))
            d->caps[method] = caps;
        d->caps[method].m_decoder = (int)d->decoder;
    }
    d->doubler = new QCheckBox(tr("Double framerate"), this);
    d->gl = new QCheckBox(tr("Use OpenGL"), this);
    d->gpu = new QCheckBox(tr("Use hardware acceleration if available"), this);
    auto hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel(tr("Method"), this));
    hbox->addWidget(d->combo);
    hbox->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    auto vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addWidget(d->doubler);
    vbox->addWidget(d->gl);
    vbox->addWidget(d->gpu);
    setLayout(vbox);

    auto update = [this] (DeintMethod method) {
        d->updating = true;
        auto &cap = d->caps[method];
        const auto &def = d->defaults[(int)method];
        const auto dev = d->hwdec ? GPU : CPU;
        d->gpu->setEnabled(d->hwdec && def.supports(GPU));
        d->gpu->setChecked(d->hwdec && cap.supports(GPU));
        d->gl->setEnabled(def.supports(dev) && def.supports(OpenGL));
        d->gl->setChecked(!def.supports(dev) || cap.supports(OpenGL));
        d->doubler->setEnabled(def.doubler());
        d->doubler->setChecked(cap.doubler());
        d->updating = false;
        cap.m_device = def.m_device;
        if (!d->gl->isChecked())
            cap.m_device &= ~OpenGL;
        if (!d->gpu->isChecked())
            cap.m_device &= ~GPU;
    };
    connect(d->combo, &DataComboBox::currentDataChanged,
            [update] (const QVariant &data) {
        update(DeintMethod(data.toInt()));
    });
    connect(d->doubler, &QCheckBox::toggled, [this] (bool on) {
        if (!d->updating) d->current().m_doubler = on;
    });
    connect(d->gl, &QCheckBox::toggled, [this] (bool on) {
        if (!d->updating) {
            if (on)
                d->current().m_device |= OpenGL;
            else
                d->current().m_device &= ~OpenGL;
        }
    });
    connect(d->gpu, &QCheckBox::toggled, [this] (bool on) {
        if (!d->updating) {
            if (on)
                d->current().m_device |= GPU;
            else
                d->current().m_device &= ~GPU;
        }
    });
    update(DeintMethod::Bob);
}

DeintWidget::~DeintWidget() {
    Record r("deint_caps");
    QStringList tokens;
    for (auto it = d->caps.begin(); it != d->caps.end(); ++it)
        tokens << it->toString();
    r.write(tokens, DecoderDeviceInfo::name(d->decoder));
    delete d;
}

auto DeintWidget::set(const DeintCaps &caps) -> void
{
    (d->caps[caps.method()] = caps).m_decoder = (int)d->decoder;
    d->combo->setCurrentData((int)caps.method());
}

auto DeintWidget::get() const -> DeintCaps
{
    return d->current();
}

auto DeintWidget::informations() -> QString
{
    auto methodText = [] (DeintMethod method, const QString &desc) -> QString
        { return DeintMethodInfo::name(method) % ": " % desc; };
    QString text =
        '\n' % tr("Methods") % "\n\n" %
        methodText(DeintMethod::Bob,
                   tr("Display each line twice.")) % '\n' %
        methodText(DeintMethod::LinearBob,
                   tr("Bob with linear interpolation.")) % '\n' %
        methodText(DeintMethod::CubicBob,
                   tr("Bob with cubic interpolation.")) % '\n' %
        methodText(DeintMethod::LinearBlend,
                   tr("Blend linearly each line with (1 2 1) filter.")) % '\n' %
        methodText(DeintMethod::Median,
                   tr("Apply median filter to every second line.")) % '\n' %
        methodText(DeintMethod::Yadif,
                   tr("Use complicated temporal and spatial interpolation."))
                   % "\n\n" %
        tr("Double framerate") % "\n\n" %
        tr("This option makes the framerate doubled. "
           "You can get smoother and fluid motions "
           "but it requires more CPU or GPU usage.") % "\n\n" %
        tr("Use OpenGL") % "\n\n" %
        tr("In most case, deinterlacing with OpenGL can be performed faster "
           "unless your graphics driver has poor support of OpenGL.") % "\n\n" %
        tr("Use hardware acceleration if available") % "\n\n" %
        tr("Some methods can be accelerated with GPU "
           "by turning on this option if your hardware supports VA-API well.")
    ;
    return text;
}
