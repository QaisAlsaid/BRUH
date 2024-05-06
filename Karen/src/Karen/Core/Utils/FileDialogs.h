#ifndef KR_FILE_DIALOGS_H
#define KR_FILE_DIALOGS_H


namespace Karen
{
  class FileDialogs
  {
  public:
    enum class OpType {OpenFile, SaveFile};
  public:
    static std::string OpenFile(const char* filters = nullptr, const char* prev = nullptr);

    static std::string SaveFile(const char* filters = nullptr, const char* prev = nullptr);
  };
}

#endif //KR_FILE_DIALOGS_H
