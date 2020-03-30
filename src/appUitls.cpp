#include <cstring>

namespace appUtils {

void decodeHTML(char* des, const char* src, int desSize) {
  for (int i = 0; i < desSize - 1 && *src; ++i, des++) {
    if (0 == strncmp("&lt;", src, 4)) {
      *des = '<';
      src += 4;
    } else if (0 == strncmp("&gt;", src, 4)) {
      *des = '>';
      src += 4;
    } else if (0 == strncmp("&amp;", src, 5)) {
      *des = '&';
      src += 5;
    } else if (0 == strncmp("&quot;", src, 6)) {
      *des = '\"';
      src += 6;
    } else if (0 == strncmp("&nbsp;", src, 6)) {
      *des = ' ';
      src += 6;
    } else {
      *des = *src;
      src++;
    }
  }

  *des = '\0';
}

void encodeHTML(char* des, const char* src, int desSize) {
  for (int i = 0; i < desSize - 1 && *src; src++) {
    if ('<' == *src) {
      if (i + 4 > desSize - 1) {
        break;
      }
      strcat(des, "&lt;");
      des += 4;
      i += 4;

    } else if ('>' == *src) {
      if (i + 4 > desSize - 1) {
        break;
      }
      strcat(des, "&gt;");
      des += 4;
      i += 4;
    } else if ('&' == *src) {
      if (i + 5 > desSize - 1) {
        break;
      }
      strcat(des, "&amp;");
      des += 5;
      i += 5;
    } else if ('\"' == *src) {
      if (i + 6 > desSize - 1) {
        break;
      }
      strcat(des, "&quot;");
      des += 6;
      i += 6;
    } else if (' ' == *src) {
      if (i + 6 > desSize - 1) {
        break;
      }
      strcat(des, "&nbsp;");
      des += 6;
      i += 6;
    } else {
      *des = *src;
      des++;
      i++;
    }
  }

  *des = '\0';
}
}  // namespace appUtils
