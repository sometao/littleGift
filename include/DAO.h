#include <string>

using std::string;


namespace littleGIft {
namespace dao {




struct SlideRow {
  int64_t id;
  string authorName;
  string content;
  string contentType;
  string accessToken;
  string editCode;
  int64_t create_time;
};






}  // namespace dao

}  // namespace littleGIft
