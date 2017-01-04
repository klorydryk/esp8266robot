char* getIndexPage() {
  return "<!DOCTYPE html>\
<html lang='fr'>\
<head>\
  <meta charset='utf-8'>\
  <script language = 'javascript' type = 'text/javascript'>\
  var wsUri = 'ws://192.168.1.28:81/';\
  var output;\
  function init() {\
    testWebSocket();\
  }\
  function testWebSocket() {\
    websocket = new WebSocket(wsUri);\
    websocket.onopen = function(evt) {\
      console.log('CONNECTED');\
    };\
    websocket.onclose = function(evt) {\
      console.log('DISCONNECTED');\
    };\
    websocket.onmessage = function(evt) {\
      console.log('RECEIVED: ' + evt.data);\
  };\
  websocket.onerror = function(evt) {\
    console.log('ERROR: ' + evt.data);\
  };\
}\
function changeMousePos(xPosition, yPosition) {\
  var xPosition = (xPosition*2)-1;\
  var yPosition = ((1-yPosition)*2)-1;\
    console.log('Y: ' + yPosition);\
  var messageX = parseInt(xPosition*1023);\
  websocket.send('X:' + messageX);\
  var messageY = parseInt(yPosition*1023  );\
  websocket.send('Y:' + messageY);\
}\
window.addEventListener('load', init, false);\
</script>\
<style type='text/css'>\
#contentContainer {\
  width: 550px;\
  height: 350px;\
  border: 5px black solid;\
  overflow: hidden;\
  background-color: #F2F2F2;\
  cursor: pointer;\
}\
</style>\
</head>\
<body>\
  <h1 align='center'>radio-command</h1>\
  <div id='contentContainer'>\
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
  <p>Uptime: %02d:%02d:%02d</p>\
  <script>\
  var container = document.querySelector('#contentContainer');\
  var isMouseDown = false;\
  container.addEventListener('mousedown', enterMouse);\
  container.addEventListener('mouseup', leaveMouse);\
  container.addEventListener('mousemove', getClickPosition);\
  function enterMouse() {\
    console.log('MOUSE: down');\
    isMouseDown = true;\
  }\
  function leaveMouse() {\
    console.log('MOUSE: up');\
    isMouseDown = false;\
  }\
  function getClickPosition(e) {\
    if(!isMouseDown)\
    return;\
    var parentPosition = getPosition(e.currentTarget);\
    var xPosition = e.clientX - parentPosition.x;\
    var yPosition = e.clientY - parentPosition.y;\
    if(xPosition < 0)\
    xPosition = 0;\
    if(xPosition > container.offsetWidth)\
    return;\
    if(yPosition < 0)\
    yPosition = 0;\
    if(yPosition > container.offsetHeight)\
    return;\
    changeMousePos(xPosition/container.offsetWidth, yPosition/container.offsetHeight);\
  }\
  // Helper function to get an element's exact position\
  function getPosition(el) {\
    var xPos = 0;\
    var yPos = 0;\
    while (el) {\
      if (el.tagName == 'BODY') {\
        // deal with browser quirks with body/window/document and page scroll\
        var xScroll = el.scrollLeft || document.documentElement.scrollLeft;\
        var yScroll = el.scrollTop || document.documentElement.scrollTop;\
        xPos += (el.offsetLeft - xScroll + el.clientLeft);\
        yPos += (el.offsetTop - yScroll + el.clientTop);\
      } else {\
        // for all other non-BODY elements\
        xPos += (el.offsetLeft - el.scrollLeft + el.clientLeft);\
        yPos += (el.offsetTop - el.scrollTop + el.clientTop);\
      }\
      el = el.offsetParent;\
    }\
    return {\
      x: xPos,\
      y: yPos\
    };\
  }\
  </script>\
</body>\
</html>";
}

