#include <Arduino.h>

class html {
  public:
    html();
    String getIndexPage() { return htmlPage; }
    int size() { return pageSize; }
    
    String getJsPage() { return jsFile; }
    int scriptSize() { return jsSize+1; }

  private:
    String makeHtml();
    String htmlPage;
    int pageSize;
    
    String makeJs();
    String jsFile;
    int jsSize;
};
