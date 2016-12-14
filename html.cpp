char* getIndexPage() {
  return "<!DOCTYPE html>\
<html lang='fr'>\
<head>\
  <meta charset='utf-8'>\
  <style type='text/css'>\
  [class|=arrow] {\
    width: 0;\
    height: 0;\
    border-left: 100px solid transparent;\
    border-right: 100px solid transparent;\
    border-bottom: 100px solid black;\
  }\
  .arrow-up {\
  }\
  .arrow-down {\
    transform: rotate(180deg);\
  }\
  .arrow-right {\
    transform: rotate(90deg);\
  }\
  .arrow-left {\
    transform: rotate(-90deg);\
  }\
  .left-right {\
    column-count: 2;\
    column-gap: 40px;\
  }\
  .stop {\
    height: 100px;\
    width: 100px;\
    background-color: red;\
  }\
  .function {\
    height: 100px;\
    width: 100px;\
    background-color: green;\
  }\
  </style>\
</head>\
<body>\
  <h1 align='center'>radio-command</h1>\
  <div align='center'>\
    <div class='row'>\
      <div>\
        <a href='?d=up'>\
          <img class='arrow-up' src=''/>\
        </a>\
      </div>\
      <div class='left-right'>\
        <a href='?d=left'>\
          <img class='arrow-left' src=''/>\
        </a>\
        <a href='?d=right'>\
          <img class='arrow-right' src=''/>\
        </a>\
      </div>\
      <div>\
        <a href='?d=down'>\
          <img class='arrow-down' src=''/>\
        </a>\
      </div>\
    </div>\
    <a href='?d=stop'>\
      <button class='stop'>\
        STOP\
      </button>\
    </a>\
    <div>\
      <a href='?d=Fn1'>\
        <button class='function'>\
          Fn1\
        </button>\
      </a>\
      <a href='?d=Fn2'>\
        <button class='function'>\
          Fn2\
        </button>\
      </a>\
    </div>\
  </div>\
  <p>Uptime: %02d:%02d:%02d</p>\
</body>\
</html>";
}

