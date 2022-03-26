#pragma once

class QSettings;
class QString;

namespace utils
{

class QSettingsKeyFetcher
{
public:
  explicit QSettingsKeyFetcher(QSettings &settings);

  QString lookup_group(QString key_name);
  QString lookup_value(QString key_name);

private:
  QSettings &m_settings;
};

} // namespace utils
