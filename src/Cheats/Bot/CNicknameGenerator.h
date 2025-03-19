#ifndef CNICKNAMEGENERATOR_H
#define CNICKNAMEGENERATOR_H

#include <string>
#include <vector>
#include <random>  // For random engine

namespace CheatModule {
 namespace Bot {

  class CNicknameGenerator {
   static std::vector<std::string> nameList;
   static std::default_random_engine randomEngine; // Declare a static random engine

  public:
   enum eStrategy {
    RANDOM_BINARY,
    RANDOM_ASCII,
    FROM_SELF,
    FROM_OTHER,
    FROM_LIST,
    FROM_LIST_SUFFIX
};

   /**
    * @param file 名字文件的路径
    * @return 是否成功读取
    */
   static bool LoadFromList(std::string file);

   /**
    * @param length 字符串的长度
    * @return 一个长度为length字符串，只含0和1
    */
   static std::string FromRandomBinary(int length);

   /**
    * @param length 字符串的长度
    * @return 一个长度为length的字符串，完全随机
    */
   static std::string FromRandomASCII(int length);

   /**
    * @param suffix 后缀长度
    * @return 一个字符串，格式为自己的名字_后缀，后缀为随机字符串
    */
   static std::string FromSelf(int suffix);

   /**
    * @param suffix 后缀长度
    * @return 一个字符串，格式为服务器里其他人的名字_后缀，后缀为随机字符串
    */
   static std::string FromOther(int suffix);

   /**
    * @return 随机从名字列表里面取出名字
    */
   static std::string FromList();
  };

 } // namespace Bot
} // namespace CheatModule

#endif //CNICKNAMEGENERATOR_H
