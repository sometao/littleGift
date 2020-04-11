#include <string>

using std::string;
namespace littleGift {
namespace dao {



struct SlidesRow {
  int64_t id;
  string authorName;
  string content;
  string contentType;
  string accessToken;
  string editCode;
  int64_t create_time;
};

int64_t addSlides(SlidesRow row);

SlidesRow getSlides(const string& token);

void initContentTable();

}  // namespace dao

}  // namespace littleGIft
