#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H


class ThemeManager
{
public:
  ThemeManager(const ThemeManager&) = delete;

  static ThemeManager& get()
  {
    return s_Instance;
  }

  void setResize(double size);
  double getResize();

private:
  ThemeManager() {}
  static ThemeManager s_Instance;

  double mClientResize = 1;
};

#endif // THEMEMANAGER_H
