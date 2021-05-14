
static const char html_header[] PROGMEM =
  "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1, user-scalable=0\">"
  "<title>FLORA - Wi-Fi VFD Clock</title><style>html,body{margin:0;padding:0;font-size:16px;background:#333;}body,*{box-sizing:border-box;font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,\"Helvetica Neue\",Arial,sans-serif;}a{color:inherit;text-decoration:underline;}.wrapper{padding:30px 0;}.container{margin:auto;padding:40px;max-width:600px;color:#fff;background:#000;box-shadow:0 0 100px rgba(0,0,0,.5);border: 3px solid #43ffe5;border-radius:35px;}.row{margin-bottom:15px;}.flexrow{display:flex;justify-items:stretch;align-items:flex-start;flex-wrap:wrap;margin-left:-10px;}.col{flex: 1;padding-left:10px;}h1{margin:0 0 10px 0;font-family:Arial,sans-serif;font-weight:300;font-size:2rem;}h1 + p{margin-bottom:30px;}h2{color:#43ffe5;margin:30px 0 0 0;font-family:Arial,sans-serif;font-weight:300;font-size:1.5rem;}h3{font-family:Arial,sans-serif;font-weight:300;font-size:1.2rem;margin: 25px 0 10px 0;}div+h3,#rgb_wrapper h3{margin-top:10px;}p{font-size:.85rem;margin:0 0 20px 0;color:rgba(255,255,255,.7);}label{display:block;width:100%;margin-bottom:5px;}label+p{margin-bottom:5px;}input[type=\"text\"],input[type=\"number\"],input[type=\"password\"],select{display:inline-block;width:100%;height:42px;line-height:38px;padding:0 20px;color:#fff;border:2px solid #666;background:none;border-radius:5px;transition:.15s;box-shadow:none;outline:none;}input[type=\"text\"]:hover,input[type=\"number\"]:hover,input[type=\"password\"]:hover,select:hover{border-color:#69b6ac;}input[type=\"text\"]:focus,input[type=\"password\"]:focus,select:focus{border-color:#a5fff3;}option{color:#000;}button{display:block;width:100%;padding:10px 20px;font-size:1rem;font-weight:700;text-transform:uppercase;background:#43ffe5;border:0;border-radius:5px;cursor:pointer;transition:.15s;outline:none;}button:hover{background:#a5fff3;}.github{padding:15px;text-align:center;}.github a{color:#43ffe5;transition:.15s;}.github a:hover{color:#a5fff3;}.github p{margin:0;}.mac{display:inline-block;margin-top:8px;padding:2px 5px;color:#fff;background:#444;border-radius:3px;}</style><style media=\"all and (max-width:620px)\">.wrapper{padding:0;}.container{padding:25px 15px;border:0;border-radius:0;}.col{flex: auto;width:50%;}.col-xs-full{flex:1;width:100%;}</style></head><body>";

static const char html_wrapper_start[] PROGMEM =
  "<div class=\"wrapper\">"
  "<div class=\"container\">";

static const char html_wrapper_end[] PROGMEM =
  "</div>"
  "<div class=\"github\"><p>"
  "{{fw_name}} {{fw_version}}"
  ", check out <a href=\"https://github.com/mcer12/Flora-ESP8266\" target=\"_blank\"><strong>FLORA</strong> on GitHub</a></p> </div>"
  "</div>";

static const char html_footer[] PROGMEM =
  "<script>"
  "function httpGetAsync(theUrl, callback){"
  "var xmlHttp = new XMLHttpRequest();"
  "xmlHttp.onreadystatechange = function(){"
  "if (xmlHttp.readyState == 4 && xmlHttp.status == 200) callback(xmlHttp.responseText);"
  "};"
  "xmlHttp.open(\"GET\", theUrl, true);"
  "xmlHttp.send(null);"
  "};"
  "var colorPickerHigh = new iro.ColorPicker('#picker_high', { width: 240,wheelAngle: 270,wheelDirection: 'clockwise',borderWidth:1,color: \"{{rgb_high}}\"});"
  "var colorPickerLow = new iro.ColorPicker('#picker_low', { width: 240,wheelAngle: 270,wheelDirection: 'clockwise',borderWidth:1,color: \"{{rgb_low}}\"});"
  "colorPickerHigh.on('input:end', function(color) {"
  "var rgbHigh = colorPickerHigh.color.rgb;"
  "document.getElementById(\"rgb_h_r\").value = rgbHigh.r;"
  "document.getElementById(\"rgb_h_g\").value = rgbHigh.g;"
  "document.getElementById(\"rgb_h_b\").value = rgbHigh.b;"
  "httpGetAsync('/?rgb_en=1&rgb_h_r='+rgbHigh.r+'&rgb_h_g='+rgbHigh.g+'&rgb_h_b='+rgbHigh.b, function(callback){console.log(callback)});"
  "});"
  "colorPickerLow.on('input:end', function(color) {"
  "var rgbLow = colorPickerLow.color.rgb;"
  "document.getElementById(\"rgb_l_r\").value = rgbLow.r;"
  "document.getElementById(\"rgb_l_g\").value = rgbLow.g;"
  "document.getElementById(\"rgb_l_b\").value = rgbLow.b;"
  "httpGetAsync('/?rgb_en=1&rgb_l_r='+rgbLow.r+'&rgb_l_g='+rgbLow.g+'&rgb_l_b='+rgbLow.b, function(callback){console.log(callback)});"
  "});"
  "function toggleVisibility(eventsender){var myNewState = \"none\";if (eventsender.checked === true){myNewState = \"block\";}document.getElementById(eventsender.dataset.target).style.display = myNewState;}"
  "var toggles = document.getElementsByClassName(\"visibility-toggle\");"
  "[].forEach.call(toggles, function (el) {toggleVisibility(el); el.addEventListener(\"click\", function(){toggleVisibility(this);}); });"
  "</script>"
  "</body> </html>";

static const char iro[] PROGMEM =
  "<script>"
  "!function(t,n){\"object\"==typeof exports&&\"undefined\"!=typeof module?module.exports=n():\"function\"==typeof define&&define.amd?define(n):(t=t||self).iro=n()}(this,function(){\"use strict\";var m,s,n,i,o,x={},j=[],r=/acit|ex(?:s|g|n|p|$)|rph|grid|ows|mnc|ntw|ine[ch]|zoo|^ord|^--/i;function M(t,n){for(var i in n)t[i]=n[i];return t}function y(t){var n=t.parentNode;n&&n.removeChild(t)}function h(t,n,i){var r,e,u,o,l=arguments;if(n=M({},n),3<arguments.length)for(i=[i],r=3;r<arguments.length;r++)i.push(l[r]);if(null!=i&&(n.children=i),null!=t&&null!=t.defaultProps)for(e in t.defaultProps)void 0===n[e]&&(n[e]=t.defaultProps[e]);return o=n.key,null!=(u=n.ref)&&delete n.ref,null!=o&&delete n.key,c(t,n,o,u)}function c(t,n,i,r){var e={type:t,props:n,key:i,ref:r,n:null,i:null,e:0,o:null,l:null,c:null,constructor:void 0};return m.vnode&&m.vnode(e),e}function O(t){return t.children}function I(t,n){this.props=t,this.context=n}function w(t,n){if(null==n)return t.i?w(t.i,t.i.n.indexOf(t)+1):null;for(var i;n<t.n.length;n++)if(null!=(i=t.n[n])&&null!=i.o)return i.o;return\"function\"==typeof t.type?w(t):null}function a(t){var n,i;if(null!=(t=t.i)&&null!=t.c){for(t.o=t.c.base=null,n=0;n<t.n.length;n++)if(null!=(i=t.n[n])&&null!=i.o){t.o=t.c.base=i.o;break}return a(t)}}function e(t){(!t.f&&(t.f=!0)&&1===s.push(t)||i!==m.debounceRendering)&&(i=m.debounceRendering,(m.debounceRendering||n)(u))}function u(){var t,n,i,r,e,u,o,l;for(s.sort(function(t,n){return n.d.e-t.d.e});t=s.pop();)t.f&&(r=i=void 0,u=(e=(n=t).d).o,o=n.p,l=n.u,n.u=!1,o&&(i=[],r=k(o,e,M({},e),n.w,void 0!==o.ownerSVGElement,null,i,l,null==u?w(e):u),d(i,e),r!=u&&a(e)))}function S(n,i,t,r,e,u,o,l,s){var c,a,f,h,v,d,g,b=t&&t.n||j,p=b.length;if(l==x&&(l=null!=u?u[0]:p?w(t,0):null),c=0,i.n=A(i.n,function(t){if(null!=t){if(t.i=i,t.e=i.e+1,null===(f=b[c])||f&&t.key==f.key&&t.type===f.type)b[c]=void 0;else for(a=0;a<p;a++){if((f=b[a])&&t.key==f.key&&t.type===f.type){b[a]=void 0;break}f=null}if(h=k(n,t,f=f||x,r,e,u,o,null,l,s),(a=t.ref)&&f.ref!=a&&(g=g||[]).push(a,t.c||h,t),null!=h){if(null==d&&(d=h),null!=t.l)h=t.l,t.l=null;else if(u==f||h!=l||null==h.parentNode){t:if(null==l||l.parentNode!==n)n.appendChild(h);"
  "else{for(v=l,a=0;(v=v.nextSibling)&&a<p;a+=2)if(v==h)break t;n.insertBefore(h,l)}\"option\"==i.type&&(n.value=\"\")}l=h.nextSibling,\"function\"==typeof i.type&&(i.l=h)}}return c++,t}),i.o=d,null!=u&&\"function\"!=typeof i.type)for(c=u.length;c--;)null!=u[c]&&y(u[c]);for(c=p;c--;)null!=b[c]&&N(b[c],b[c]);if(g)for(c=0;c<g.length;c++)E(g[c],g[++c],g[++c])}function A(t,n,i){if(null==i&&(i=[]),null==t||\"boolean\"==typeof t)n&&i.push(n(null));else if(Array.isArray(t))for(var r=0;r<t.length;r++)A(t[r],n,i);else i.push(n?n(function(t){if(null==t||\"boolean\"==typeof t)return null;if(\"string\"==typeof t||\"number\"==typeof t)return c(null,t,null,null);if(null==t.o&&null==t.c)return t;var n=c(t.type,t.props,t.key,null);return n.o=t.o,n}(t)):t);return i}function f(t,n,i){\"-\"===n[0]?t.setProperty(n,i):t[n]=\"number\"==typeof i&&!1===r.test(n)?i+\"px\":null==i?\"\":i}function R(t,n,i,r,e){var u,o,l,s,c;if(\"key\"===(n=e?\"className\"===n?\"class\":n:\"class\"===n?\"className\":n)||\"children\"===n);else if(\"style\"===n)if(u=t.style,\"string\"==typeof i)u.cssText=i;else{if(\"string\"==typeof r&&(u.cssText=\"\",r=null),r)for(o in r)i&&o in i||f(u,o,\"\");if(i)for(l in i)r&&i[l]===r[l]||f(u,l,i[l])}else\"o\"===n[0]&&\"n\"===n[1]?(s=n!==(n=n.replace(/Capture$/,\"\")),n=((c=n.toLowerCase())in t?c:n).slice(2),i?(r||t.addEventListener(n,v,s),(t.t||(t.t={}))[n]=i):t.removeEventListener(n,v,s)):\"list\"!==n&&\"tagName\"!==n&&\"form\"!==n&&!e&&n in t?t[n]=null==i?\"\":i:\"function\"!=typeof i&&\"dangerouslySetInnerHTML\"!==n&&(n!==(n=n.replace(/^xlink:?/,\"\"))?null==i||!1===i?t.removeAttributeNS(\"http://www.w3.org/1999/xlink\",n.toLowerCase()):t.setAttributeNS(\"http://www.w3.org/1999/xlink\",n.toLowerCase(),i):null==i||!1===i?t.removeAttribute(n):t.setAttribute(n,i))}function v(t){return this.t[t.type](m.event?m.event(t):t)}function k(t,n,i,r,e,u,o,l,s,c){var a,f,h,v,d,g,b,p,y,w,k=n.type;if(void 0!==n.constructor)return null;(a=m.e)&&a(n);try{t:if(\"function\"==typeof k)"
  "{if(p=n.props,y=(a=k.contextType)&&r[a.c],w=a?y?y.props.value:a.i:r,i.c?b=(f=n.c=i.c).i=f.k:(\"prototype\"in k&&k.prototype.render?n.c=f=new k(p,w):(n.c=f=new I(p,w),f.constructor=k,f.render=z),y&&y.sub(f),f.props=p,f.state||(f.state={}),f.context=w,f.w=r,h=f.f=!0,f.m=[]),null==f.j&&(f.j=f.state),null!=k.getDerivedStateFromProps&&M(f.j==f.state?f.j=M({},f.j):f.j,k.getDerivedStateFromProps(p,f.j)),h)null==k.getDerivedStateFromProps&&null!=f.componentWillMount&&f.componentWillMount(),null!=f.componentDidMount&&o.push(f);else{if(null==k.getDerivedStateFromProps&&null==l&&null!=f.componentWillReceiveProps&&f.componentWillReceiveProps(p,w),!l&&null!=f.shouldComponentUpdate&&!1===f.shouldComponentUpdate(p,f.j,w)){for(f.props=p,f.state=f.j,f.f=!1,(f.d=n).o=null!=s?s!==i.o?s:i.o:null,n.n=i.n,a=0;a<n.n.length;a++)n.n[a]&&(n.n[a].i=n);break t}null!=f.componentWillUpdate&&f.componentWillUpdate(p,f.j,w)}for(v=f.props,d=f.state,f.context=w,f.props=p,f.state=f.j,(a=m.M)&&a(n),f.f=!1,f.d=n,f.p=t,a=f.render(f.props,f.state,f.context),n.n=A(null!=a&&a.type==O&&null==a.key?a.props.children:a),null!=f.getChildContext&&(r=M(M({},r),f.getChildContext())),h||null==f.getSnapshotBeforeUpdate||(g=f.getSnapshotBeforeUpdate(v,d)),S(t,n,i,r,e,u,o,s,c),f.base=n.o;a=f.m.pop();)f.j&&(f.state=f.j),a.call(f);h||null==v||null==f.componentDidUpdate||f.componentDidUpdate(v,d,g),b&&(f.k=f.i=null)}else n.o=function(t,n,i,r,e,u,o,l){var s,c,a,f,h=i.props,v=n.props;if(e=\"svg\"===n.type||e,null==t&&null!=u)for(s=0;s<u.length;s++)if(null!=(c=u[s])&&(null===n.type?3===c.nodeType:c.localName===n.type)){t=c,u[s]=null;break}if(null==t){if(null===n.type)return document.createTextNode(v);t=e?document.createElementNS(\"http://www.w3.org/2000/svg\",n.type):document.createElement(n.type),u=null}return null===n.type?h!==v&&(null!=u&&(u[u.indexOf(t)]=null),t.data=v):n!==i&&(null!=u&&(u=j.slice.call(t.childNodes)),a=(h=i.props||x).dangerouslySetInnerHTML,f=v.dangerouslySetInnerHTML,l||(f||a)&&(f&&a&&f.O==a.O||(t.innerHTML=f&&f.O||\"\")),function(t,n,i,r,e){var u;for(u in i)u in n||R(t,u,null,i[u],r);for(u in n)e&&\"function\"!=typeof n[u]||\"value\"===u||\"checked\"===u||i[u]===n[u]||R(t,u,n[u],i[u],r)}"
  "(t,v,h,e,l),n.n=n.props.children,f||S(t,n,i,r,\"foreignObject\"!==n.type&&e,u,o,x,l),l||(\"value\"in v&&void 0!==v.value&&v.value!==t.value&&(t.value=null==v.value?\"\":v.value),\"checked\"in v&&void 0!==v.checked&&v.checked!==t.checked&&(t.checked=v.checked))),t}(i.o,n,i,r,e,u,o,c);(a=m.diffed)&&a(n)}catch(t){m.o(t,n,i)}return n.o}function d(t,n){for(var i;i=t.pop();)try{i.componentDidMount()}catch(t){m.o(t,i.d)}m.c&&m.c(n)}function E(t,n,i){try{\"function\"==typeof t?t(n):t.current=n}catch(t){m.o(t,i)}}function N(t,n,i){var r,e,u;if(m.unmount&&m.unmount(t),(r=t.ref)&&E(r,null,n),i||\"function\"==typeof t.type||(i=null!=(e=t.o)),t.o=t.l=null,null!=(r=t.c)){if(r.componentWillUnmount)try{r.componentWillUnmount()}catch(t){m.o(t,n)}r.base=r.p=null}if(r=t.n)for(u=0;u<r.length;u++)r[u]&&N(r[u],n,i);null!=e&&y(e)}function z(t,n,i){return this.constructor(t,i)}function g(t,n){for(var i=0;i<n.length;i++){var r=n[i];r.enumerable=r.enumerable||!1,r.configurable=!0,\"value\"in r&&(r.writable=!0),Object.defineProperty(t,r.key,r)}}function b(){return(b=Object.assign||function(t){for(var n=arguments,i=1;i<arguments.length;i++){var r=n[i];for(var e in r)Object.prototype.hasOwnProperty.call(r,e)&&(t[e]=r[e])}return t}).apply(this,arguments)}m={},I.prototype.setState=function(t,n){var i=this.j!==this.state&&this.j||(this.j=M({},this.state));\"function\"==typeof t&&!(t=t(i,this.props))||M(i,t),null!=t&&this.d&&(this.u=!1,n&&this.m.push(n),e(this))},I.prototype.forceUpdate=function(t){this.d&&(t&&this.m.push(t),this.u=!0,e(this))},I.prototype.render=O,s=[],n=\"function\"==typeof Promise?Promise.prototype.then.bind(Promise.resolve()):setTimeout,i=m.debounceRendering,m.o=function(t,n,i){for(var r;n=n.i;)if((r=n.c)&&!r.i)try{if(r.constructor&&null!=r.constructor.getDerivedStateFromError)r.setState(r.constructor.getDerivedStateFromError(t));"
  "else{if(null==r.componentDidCatch)continue;r.componentDidCatch(t)}return e(r.k=r)}catch(n){t=n}throw t},o=x;var t=\"(?:[-\\\\+]?\\\\d*\\\\.\\\\d+%?)|(?:[-\\\\+]?\\\\d+%?)\",l=\"[\\\\s|\\\\(]+(\"+t+\")[,|\\\\s]+(\"+t+\")[,|\\\\s]+(\"+t+\")\\\\s*\\\\)?\",p=\"[\\\\s|\\\\(]+(\"+t+\")[,|\\\\s]+(\"+t+\")[,|\\\\s]+(\"+t+\")[,|\\\\s]+(\"+t+\")\\\\s*\\\\)?\",_=new RegExp(\"rgb\"+l),H=new RegExp(\"rgba\"+p),P=new RegExp(\"hsl\"+l),$=new RegExp(\"hsla\"+p),T=\"^(?:#?|0x?)\",W=\"([0-9a-fA-F]{1})\",C=\"([0-9a-fA-F]{2})\",D=new RegExp(T+W+W+W+\"$\"),F=new RegExp(T+W+W+W+W+\"$\"),L=new RegExp(T+C+C+C+\"$\"),B=new RegExp(T+C+C+C+C+\"$\"),q=Math.log,G=Math.round,Z=Math.floor;function J(t,n,i){return Math.min(Math.max(t,n),i)}function K(t,n){var i=-1<t.indexOf(\"%\"),r=parseFloat(t);return i?n/100*r:r}function Q(t){return parseInt(t,16)}function U(t){return t.toString(16).padStart(2,\"0\")}var V=function(){function l(t,n){this.$={h:0,s:0,v:0,a:1},t&&this.set(t),this.onChange=n,this.initialValue=b({},this.$)}var t=l.prototype;return t.set=function(t){if(\"string\"==typeof t)/^(?:#?|0x?)[0-9a-fA-F]{3,8}$/.test(t)?this.hexString=t:/^rgba?/.test(t)?this.rgbString=t:/^hsla?/.test(t)&&(this.hslString=t);else{if(\"object\"!=typeof t)throw new Error(\"Invalid color value\");t instanceof l?this.hsv=t.hsv:\"r\"in t&&\"g\"in t&&\"b\"in t?this.rgb=t:\"h\"in t&&\"s\"in t&&\"v\"in t?this.hsv=t:\"h\"in t&&\"s\"in t&&\"l\"in t?this.hsl=t:\"kelvin\"in t&&(this.kelvin=t.kelvin)}},t.setChannel=function(t,n,i){var r;this[t]=b({},this[t],((r={})[n]=i,r))},t.reset=function(){this.hsva=this.initialValue},t.clone=function(){return new l(this)},t.unbind=function(){this.onChange=void 0},l.hsvToRgb=function(t){var n=t.h/60,i=t.s/100,r=t.v/100,e=Z(n),u=n-e,o=r*(1-i),l=r*(1-u*i),s=r*(1-(1-u)*i),c=e%6,a=[s,r,r,l,o,o][c],f=[o,o,s,r,r,l][c];return{r:J(255*[r,l,o,o,s,r][c],0,255),g:J(255*a,0,255),b:J(255*f,0,255)}},l.rgbToHsv=function(t){var n=t.r/255,i=t.g/255,r=t.b/255,e=Math.max(n,i,r),u=Math.min(n,i,r),o=e-u,l=0,s=e,c=0===e?0:o/e;switch(e){case u:l=0;break;case n:l=(i-r)/o+(i<r?6:0);break;case i:l=(r-n)/o+2;break;case r:l=(n-i)/o+4}return{h:60*l%360,s:J(100*c,0,100),v:J(100*s,0,100)}},l.hsvToHsl=function(t)"
  "{var n=t.s/100,i=t.v/100,r=(2-n)*i,e=r<=1?r:2-r,u=e<1e-9?0:n*i/e;return{h:t.h,s:J(100*u,0,100),l:J(50*r,0,100)}},l.hslToHsv=function(t){var n=2*t.l,i=t.s*(n<=100?n:200-n)/100,r=n+i<1e-9?0:2*i/(n+i);return{h:t.h,s:J(100*r,0,100),v:J((n+i)/2,0,100)}},l.kelvinToRgb=function(t){var n,i,r,e=t/100;return r=e<66?(n=255,i=-155.25485562709179-.44596950469579133*(i=e-2)+104.49216199393888*q(i),e<20?0:.8274096064007395*(r=e-10)-254.76935184120902+115.67994401066147*q(r)):(n=351.97690566805693+.114206453784165*(n=e-55)-40.25366309332127*q(n),i=325.4494125711974+.07943456536662342*(i=e-50)-28.0852963507957*q(i),255),{r:J(Z(n),0,255),g:J(Z(i),0,255),b:J(Z(r),0,255)}},l.rgbToKelvin=function(t){for(var n,i=t.r,r=t.b,e=2e3,u=4e4;.4<u-e;){var o=l.kelvinToRgb(n=.5*(u+e));o.b/o.r>=r/i?u=n:e=n}return n},function(t,n,i){n&&g(t.prototype,n),i&&g(t,i)}(l,[{key:\"hsv\",get:function(){var t=this.$;return{h:t.h,s:t.s,v:t.v}},set:function(t){var n=this.$;if(t=b({},n,t),this.onChange){var i={h:!1,v:!1,s:!1,a:!1};for(var r in n)i[r]=t[r]!=n[r];this.$=t,(i.h||i.s||i.v||i.a)&&this.onChange(this,i)}else this.$=t}},{key:\"hsva\",get:function(){return b({},this.$)},set:function(t){this.hsv=t}},{key:\"hue\",get:function(){return this.$.h},set:function(t){this.hsv={h:t}}},{key:\"saturation\",get:function(){return this.$.s},set:function(t){this.hsv={s:t}}},{key:\"value\",get:function(){return this.$.v},set:function(t){this.hsv={v:t}}},{key:\"alpha\",get:function(){return this.$.a},set:function(t){this.hsv=b({},this.hsv,{a:t})}},{key:\"kelvin\",get:function(){return l.rgbToKelvin(this.rgb)},set:function(t){this.rgb=l.kelvinToRgb(t)}},{key:\"red\",get:function(){return this.rgb.r},set:function(t){this.rgb=b({},this.rgb,{r:t})}},{key:\"green\",get:function(){return this.rgb.g},set:function(t){this.rgb=b({},this.rgb,{g:t})}},{key:\"blue\",get:function(){return this.rgb.b},set:function(t){this.rgb=b({},this.rgb,{b:t})}},{key:\"rgb\",get:function(){var t=l.hsvToRgb(this.$),n=t.r,i=t.g,r=t.b;return{r:G(n),g:G(i),b:G(r)}},set:function(t){this.hsv=b({},l.rgbToHsv(t),{a:void 0===t.a?1:t.a})}},{key:\"rgba\",get:function(){return b({},this.rgb,{a:this.alpha})},set:function(t){this.rgb=t}},"
  "{key:\"hsl\",get:function(){var t=l.hsvToHsl(this.$),n=t.h,i=t.s,r=t.l;return{h:G(n),s:G(i),l:G(r)}},set:function(t){this.hsv=b({},l.hslToHsv(t),{a:void 0===t.a?1:t.a})}},{key:\"hsla\",get:function(){return b({},this.hsl,{a:this.alpha})},set:function(t){this.hsl=t}},{key:\"rgbString\",get:function(){var t=this.rgb;return\"rgb(\"+t.r+\", \"+t.g+\", \"+t.b+\")\"},set:function(t){var n,i,r,e,u=1;if((n=_.exec(t))?(i=K(n[1],255),r=K(n[2],255),e=K(n[3],255)):(n=H.exec(t))&&(i=K(n[1],255),r=K(n[2],255),e=K(n[3],255),u=K(n[4],1)),!n)throw new Error(\"Invalid rgb string\");this.rgb={r:i,g:r,b:e,a:u}}},{key:\"rgbaString\",get:function(){var t=this.rgba;return\"rgba(\"+t.r+\", \"+t.g+\", \"+t.b+\", \"+t.a+\")\"},set:function(t){this.rgbString=t}},{key:\"hexString\",get:function(){var t=this.rgb;return\"#\"+U(t.r)+U(t.g)+U(t.b)},set:function(t){var n,i,r,e,u=255;if((n=D.exec(t))?(i=17*Q(n[1]),r=17*Q(n[2]),e=17*Q(n[3])):(n=F.exec(t))?(i=17*Q(n[1]),r=17*Q(n[2]),e=17*Q(n[3]),u=17*Q(n[4])):(n=L.exec(t))?(i=Q(n[1]),r=Q(n[2]),e=Q(n[3])):(n=B.exec(t))&&(i=Q(n[1]),r=Q(n[2]),e=Q(n[3]),u=Q(n[4])),!n)throw new Error(\"Invalid hex string\");this.rgb={r:i,g:r,b:e,a:u/255}}},{key:\"hex8String\",get:function(){var t=this.rgba;return\"#\"+U(t.r)+U(t.g)+U(t.b)+U(Z(255*t.a))},set:function(t){this.hexString=t}},{key:\"hslString\",get:function(){var t=this.hsl;return\"hsl(\"+t.h+\", \"+t.s+\"%, \"+t.l+\"%)\"},set:function(t){var n,i,r,e,u=1;if((n=P.exec(t))?(i=K(n[1],360),r=K(n[2],100),e=K(n[3],100)):(n=$.exec(t))&&(i=K(n[1],360),r=K(n[2],100),e=K(n[3],100),u=K(n[4],1)),!n)throw new Error(\"Invalid hsl string\");this.hsl={h:i,s:r,l:e,a:u}}},{key:\"hslaString\",get:function(){var t=this.hsla;return\"hsl(\"+t.h+\", \"+t.s+\"%, \"+t.l+\"%, \"+t.a+\")\"},set:function(t){this.hslString=t}}]),l}();function X(t){var n,i=t.width,r=t.sliderSize,e=t.borderWidth,u=t.handleRadius,o=t.padding,l=t.sliderShape,s=\"horizontal\"===t.layoutDirection;return r=null!=(n=r)?n:2*o+2*u,\"circle\"===l?{handleStart:t.padding+t.handleRadius,handleRange:i-2*o-2*u,width:i,height:i,cx:i/2,cy:i/2,radius:i/2-e/2}:{handleStart:r/2,handleRange:i-r,radius:r/2,x:0,y:0,width:s?r:i,height:s?i:r}}"
  "function Y(t,n){var i=X(t),r=i.width,e=i.height,u=i.handleRange,o=i.handleStart,l=\"horizontal\"===t.layoutDirection,s=l?r/2:e/2,c=o+function(t,n){var i=n.hsva,r=n.rgb;switch(t.sliderType){case\"red\":return r.r/2.55;case\"green\":return r.g/2.55;case\"blue\":return r.b/2.55;case\"alpha\":return 100*i.a;case\"kelvin\":var e=t.minTemperature,u=t.maxTemperature-e,o=(n.kelvin-e)/u*100;return Math.max(0,Math.min(o,100));case\"hue\":return i.h/=3.6;case\"saturation\":return i.s;case\"value\":default:return i.v}}(t,n)/100*u;return l&&(c=-1*c+u+2*o),{x:l?s:c,y:l?c:s}}var tt,nt=2*Math.PI,it=function(t,n){return(t%n+n)%n},rt=function(t,n){return Math.sqrt(t*t+n*n)};function et(t){return t.width/2-t.padding-t.handleRadius-t.borderWidth}function ut(t){var n=t.width/2;return{width:t.width,radius:n-t.borderWidth,cx:n,cy:n}}function ot(t,n,i){var r=t.wheelAngle,e=t.wheelDirection;return i&&\"clockwise\"===e?n=r+n:\"clockwise\"===e?n=360-r+n:i&&\"anticlockwise\"===e?n=r+180-n:\"anticlockwise\"===e&&(n=r-n),it(n,360)}function lt(t,n,i){var r=ut(t),e=r.cx,u=r.cy,o=et(t);n=e-n,i=u-i;var l=ot(t,Math.atan2(-i,-n)*(360/nt)),s=Math.min(rt(n,i),o);return{h:Math.round(l),s:Math.round(100/o*s)}}function st(t){var n=t.width,i=t.boxHeight;return{width:n,height:null!=i?i:n,radius:t.padding+t.handleRadius}}function ct(t,n,i){var r=st(t),e=r.width,u=r.height,o=r.radius,l=(n-o)/(e-2*o)*100,s=(i-o)/(u-2*o)*100;return{s:Math.max(0,Math.min(l,100)),v:Math.max(0,Math.min(100-s,100))}}function at(t,n,i,r){for(var e=0;e<r.length;e++){var u=r[e].x-n,o=r[e].y-i;if(Math.sqrt(u*u+o*o)<t.handleRadius)return e}return null}function ft(t){return{boxSizing:\"border-box\",border:t.borderWidth+\"px solid \"+t.borderColor}}function ht(t,n,i){return t+\"-gradient(\"+n+\", \"+i.map(function(t){var n=t[0];return t[1]+\" \"+n+\"%\"}).join(\",\")+\")\"}function vt(t){return\"string\"==typeof t?t:t+\"px\"}var dt=[\"mousemove\",\"touchmove\",\"mouseup\",\"touchend\"],gt=function(n){function t(t){n.call(this,t),this.uid=(Math.random()+1).toString(36).substring(5)}"
  "return n&&(t.__proto__=n),((t.prototype=Object.create(n&&n.prototype)).constructor=t).prototype.render=function(t){var n=this.handleEvent.bind(this),i={onMouseDown:n,ontouchstart:n},r=\"horizontal\"===t.layoutDirection,e=null===t.margin?t.sliderMargin:t.margin,u={overflow:\"visible\",display:r?\"inline-block\":\"block\"};return 0<t.index&&(u[r?\"marginLeft\":\"marginTop\"]=e),h(O,null,t.children(this.uid,i,u))},t.prototype.handleEvent=function(t){var n=this,i=this.props.onInput,r=this.base.getBoundingClientRect();t.preventDefault();var e=t.touches?t.changedTouches[0]:t,u=e.clientX-r.left,o=e.clientY-r.top;switch(t.type){case\"mousedown\":case\"touchstart\":!1!==i(u,o,0)&&dt.forEach(function(t){document.addEventListener(t,n,{passive:!1})});break;case\"mousemove\":case\"touchmove\":i(u,o,1);break;case\"mouseup\":case\"touchend\":i(u,o,2),dt.forEach(function(t){document.removeEventListener(t,n,{passive:!1})})}},t}(I);function bt(t){var n=t.r,i=t.url,r=n,e=n;return h(\"svg\",{className:\"IroHandle IroHandle--\"+t.index+\" \"+(t.isActive?\"IroHandle--isActive\":\"\"),style:{transform:\"translate(\"+vt(t.x)+\", \"+vt(t.y)+\")\",willChange:\"transform\",top:vt(-n),left:vt(-n),width:vt(2*n),height:vt(2*n),position:\"absolute\",overflow:\"visible\"}},i&&h(\"use\",Object.assign({xlinkHref:function(t){tt=tt||document.getElementsByTagName(\"base\");var n=window.navigator.userAgent,i=/^((?!chrome|android).)*safari/i.test(n),r=/iPhone|iPod|iPad/i.test(n),e=window.location;return(i||r)&&0<tt.length?e.protocol+\"//\"+e.host+e.pathname+e.search+t:t}(i)},t.props)),!i&&h(\"circle\",{cx:r,cy:e,r:n,fill:\"none\",\"stroke-width\":2,stroke:\"#000\"}),!i&&h(\"circle\",{cx:r,cy:e,r:n-2,fill:t.fill,\"stroke-width\":2,stroke:\"#fff\"}))}function pt(e){var t=e.activeIndex,u=void 0!==t&&t<e.colors.length?e.colors[t]:e.color,n=X(e),r=n.width,o=n.height,l=n.radius,s=Y(e,u),c=function(t,n){var i=n.hsv,r=n.rgb;switch(t.sliderType){case\"red\":return[[0,\"rgb(0,\"+r.g+\",\"+r.b+\")\"],[100,\"rgb(255,\"+r.g+\",\"+r.b+\")\"]];case\"green\":return[[0,\"rgb(\"+r.r+\",0,\"+r.b+\")\"],[100,\"rgb(\"+r.r+\",255,\"+r.b+\")\"]];case\"blue\":return[[0,\"rgb(\"+r.r+\",\"+r.g+\",0)\"],"
  "[100,\"rgb(\"+r.r+\",\"+r.g+\",255)\"]];case\"alpha\":return[[0,\"rgba(\"+r.r+\",\"+r.g+\",\"+r.b+\",0)\"],[100,\"rgb(\"+r.r+\",\"+r.g+\",\"+r.b+\")\"]];case\"kelvin\":for(var e=[],u=t.minTemperature,o=t.maxTemperature,l=o-u,s=u,c=0;s<o;s+=l/8,c+=1){var a=V.kelvinToRgb(s),f=a.r,h=a.g,v=a.b;e.push([12.5*c,\"rgb(\"+f+\",\"+h+\",\"+v+\")\"])}return e;case\"hue\":return[[0,\"#f00\"],[16.666,\"#ff0\"],[33.333,\"#0f0\"],[50,\"#0ff\"],[66.666,\"#00f\"],[83.333,\"#f0f\"],[100,\"#f00\"]];case\"saturation\":var d=V.hsvToHsl({h:i.h,s:0,v:i.v}),g=V.hsvToHsl({h:i.h,s:100,v:i.v});return[[0,\"hsl(\"+d.h+\",\"+d.s+\"%,\"+d.l+\"%)\"],[100,\"hsl(\"+g.h+\",\"+g.s+\"%,\"+g.l+\"%)\"]];case\"value\":default:var b=V.hsvToHsl({h:i.h,s:i.s,v:100});return[[0,\"#000\"],[100,\"hsl(\"+b.h+\",\"+b.s+\"%,\"+b.l+\"%)\"]]}}(e,u);return h(gt,Object.assign({},e,{onInput:function(t,n,i){var r=function(t,n,i){var r,e=X(t),u=e.handleRange,o=e.handleStart;r=\"horizontal\"===t.layoutDirection?-1*i+u+o:n-o,r=Math.max(Math.min(r,u),0);var l=Math.round(100/u*r);switch(t.sliderType){case\"kelvin\":var s=t.minTemperature;return s+l/100*(t.maxTemperature-s);case\"alpha\":return l/100;case\"hue\":return 3.6*l;case\"red\":case\"blue\":case\"green\":return 2.55*l;default:return l}}(e,t,n);e.parent.inputActive=!0,u[e.sliderType]=r,e.onInput(i,e.id)}}),function(t,n,i){return h(\"div\",Object.assign({},n,{className:\"IroSlider\",style:Object.assign({},{position:\"relative\",width:vt(r),height:vt(o),borderRadius:vt(l),background:\"conic-gradient(#ccc 25%, #fff 0 50%, #ccc 0 75%, #fff 0)\",backgroundSize:\"8px 8px\"},i)}),h(\"div\",{className:\"IroSliderGradient\",style:Object.assign({},{position:\"absolute\",top:0,left:0,width:\"100%\",height:\"100%\",borderRadius:vt(l),background:ht(\"linear\",\"horizontal\"===e.layoutDirection?\"to top\":\"to right\",c)},ft(e))}),h(bt,{isActive:!0,index:u.index,r:e.handleRadius,url:e.handleSvg,props:e.handleProps,x:s.x,y:s.y}))})}function yt(e){var t=st(e),r=t.width,u=t.height,o=t.radius,l=e.colors,s=e.parent,n=e.activeIndex,c=void 0!==n&&n<e.colors.length?e.colors[n]:e.color,a=function(t,n){return[[[0,\"#fff\"],[100,\"hsl(\"+n.hue+\",100%,50%)\"]],[[0,\"rgba(0,0,0,0)\"],"
  "[100,\"#000\"]]]}(0,c),f=l.map(function(t){return function(t,n){var i=st(t),r=i.width,e=i.height,u=i.radius,o=n.hsv,l=u,s=r-2*u,c=e-2*u;return{x:l+o.s/100*s,y:l+(c-o.v/100*c)}}(e,t)});return h(gt,Object.assign({},e,{onInput:function(t,n,i){if(0===i){var r=at(e,t,n,f);null!==r?s.setActiveColor(r):(s.inputActive=!0,c.hsv=ct(e,t,n),e.onInput(i,e.id))}else 1===i&&(s.inputActive=!0,c.hsv=ct(e,t,n));e.onInput(i,e.id)}}),function(t,n,i){return h(\"div\",Object.assign({},n,{className:\"IroBox\",style:Object.assign({},{width:vt(r),height:vt(u),position:\"relative\"},i)}),h(\"div\",{className:\"IroBox\",style:Object.assign({},{width:\"100%\",height:\"100%\",borderRadius:vt(o)},ft(e),{background:ht(\"linear\",\"to bottom\",a[1])+\",\"+ht(\"linear\",\"to right\",a[0])})}),l.filter(function(t){return t!==c}).map(function(t){return h(bt,{isActive:!1,index:t.index,fill:t.hslString,r:e.handleRadius,url:e.handleSvg,props:e.handleProps,x:f[t.index].x,y:f[t.index].y})}),h(bt,{isActive:!0,index:c.index,fill:c.hslString,r:e.activeHandleRadius||e.handleRadius,url:e.handleSvg,props:e.handleProps,x:f[c.index].x,y:f[c.index].y}))})}bt.defaultProps={fill:\"none\",x:0,y:0,r:8,url:null,props:{x:0,y:0}},pt.defaultProps=Object.assign({},{sliderShape:\"bar\",sliderType:\"value\",minTemperature:2200,maxTemperature:11e3});function wt(e){var r=ut(e).width,u=e.colors,o=(e.borderWidth,e.parent),l=e.color,s=l.hsv,c=u.map(function(t){return function(t,n){var i=n.hsv,r=ut(t),e=r.cx,u=r.cy,o=et(t),l=(180+ot(t,i.h,!0))*(nt/360),s=i.s/100*o,c=\"clockwise\"===t.wheelDirection?-1:1;return{x:e+s*Math.cos(l)*c,y:u+s*Math.sin(l)*c}}(e,t)}),a={position:\"absolute\",top:0,left:0,width:\"100%\",height:\"100%\",borderRadius:\"50%\",boxSizing:\"border-box\"};return h(gt,Object.assign({},e,{onInput:function(t,n,i){if(0===i){if(!function(t,n,i){var r=ut(t),e=r.cx,u=r.cy,o=t.width/2;return rt(e-n,u-i)<o}(e,t,n))return!1;var r=at(e,t,n,c);null!==r?o.setActiveColor(r):(o.inputActive=!0,l.hsv=lt(e,t,n),e.onInput(i,e.id))}else 1===i&&(o.inputActive=!0,l.hsv=lt(e,t,n));e.onInput(i,e.id)}}),function(t,n,i){return h(\"div\",Object.assign({},n,{className:\"IroWheel\",style:Object.assign({},{width:vt(r),height:vt(r),"
  "position:\"relative\"},i)}),h(\"div\",{className:\"IroWheelHue\",style:Object.assign({},a,{transform:\"rotateZ(\"+(e.wheelAngle+90)+\"deg)\",background:\"clockwise\"===e.wheelDirection?\"conic-gradient(red, yellow, lime, aqua, blue, magenta, red)\":\"conic-gradient(red, magenta, blue, aqua, lime, yellow, red)\"})}),h(\"div\",{className:\"IroWheelSaturation\",style:Object.assign({},a,{background:\"radial-gradient(circle closest-side, #fff, transparent)\"})}),e.wheelLightness&&h(\"div\",{className:\"IroWheelLightness\",style:Object.assign({},a,{background:\"#000\",opacity:1-s.v/100})}),h(\"div\",{className:\"IroWheelBorder\",style:Object.assign({},a,ft(e))}),u.filter(function(t){return t!==l}).map(function(t){return h(bt,{isActive:!1,index:t.index,fill:t.hslString,r:e.handleRadius,url:e.handleSvg,props:e.handleProps,x:c[t.index].x,y:c[t.index].y})}),h(bt,{isActive:!0,index:l.index,fill:l.hslString,r:e.activeHandleRadius||e.handleRadius,url:e.handleSvg,props:e.handleProps,x:c[l.index].x,y:c[l.index].y}))})}var kt=function(i){function t(t){var n=this;i.call(this,t),this.colors=[],this.inputActive=!1,this.events={},this.activeEvents={},this.deferredEvents={},this.id=t.id,(0<t.colors.length?t.colors:[t.color]).forEach(function(t){return n.addColor(t)}),this.setActiveColor(0),this.state=Object.assign({},t,{color:this.color,colors:this.colors,layout:t.layout})}return i&&(t.__proto__=i),((t.prototype=Object.create(i&&i.prototype)).constructor=t).prototype.addColor=function(t,n){void 0===n&&(n=this.colors.length);var i=new V(t,this.onColorChange.bind(this));this.colors.splice(n,0,i),this.colors.forEach(function(t,n){return t.index=n}),this.state&&this.setState({colors:this.colors}),this.deferredEmit(\"color:init\",i)},t.prototype.removeColor=function(t){var n=this.colors.splice(t,1)[0];n.unbind(),this.colors.forEach(function(t,n){return t.index=n}),this.state&&this.setState({colors:this.colors}),n.index===this.color.index&&this.setActiveColor(0),this.emit(\"color:remove\",n)},t.prototype.setActiveColor=function(t){this.color=this.colors[t],this.state&&this.setState({color:this.color}),this.emit(\"color:setActive\",this.color)},"
  "t.prototype.setColors=function(t,n){var i=this;void 0===n&&(n=0),this.colors.forEach(function(t){return t.unbind()}),this.colors=[],t.forEach(function(t){return i.addColor(t)}),this.setActiveColor(n),this.emit(\"color:setAll\",this.colors)},t.prototype.on=function(t,n){var i=this,r=this.events;(Array.isArray(t)?t:[t]).forEach(function(t){(r[t]||(r[t]=[])).push(n),i.deferredEvents[t]&&(i.deferredEvents[t].forEach(function(t){n.apply(null,t)}),i.deferredEvents[t]=[])})},t.prototype.off=function(t,i){var r=this;(Array.isArray(t)?t:[t]).forEach(function(t){var n=r.events[t];n&&n.splice(n.indexOf(i),1)})},t.prototype.emit=function(t){for(var n=this,i=[],r=arguments.length-1;0<r--;)i[r]=arguments[r+1];var e=this.activeEvents;!!e.hasOwnProperty(t)&&e[t]||(e[t]=!0,(this.events[t]||[]).forEach(function(t){return t.apply(n,i)}),e[t]=!1)},t.prototype.deferredEmit=function(t){for(var n,i=[],r=arguments.length-1;0<r--;)i[r]=arguments[r+1];var e=this.deferredEvents;(n=this).emit.apply(n,[t].concat(i)),(e[t]||(e[t]=[])).push(i)},t.prototype.setOptions=function(t){this.setState(t)},t.prototype.resize=function(t){this.setOptions({width:t})},t.prototype.reset=function(){this.colors.forEach(function(t){return t.reset()}),this.setState({colors:this.colors})},t.prototype.onMount=function(t){this.el=t,this.deferredEmit(\"mount\",this)},t.prototype.onColorChange=function(t,n){this.setState({color:this.color}),this.inputActive&&(this.inputActive=!1,this.emit(\"input:change\",t,n)),this.emit(\"color:change\",t,n)},t.prototype.emitInputEvent=function(t,n){0===t?this.emit(\"input:start\",this.color,n):1===t?this.emit(\"input:move\",this.color,n):2===t&&this.emit(\"input:end\",this.color,n)},t.prototype.render=function(t,e){var u=this,n=e.layout;return Array.isArray(n)||(n=[{component:wt},{component:pt}],e.transparency&&n.push({component:pt,options:{sliderType:\"alpha\"}})),h(\"div\",{class:\"IroColorPicker\",id:e.id,style:{display:e.display}},n.map(function(t,n){var i=t.component,r=t.options;return h(i,Object.assign({},e,r,{ref:void 0,onInput:u.emitInputEvent.bind(u),parent:u,index:n}))}))},t}(I);kt.defaultProps=Object.assign({},"
  "{width:300,height:300,color:\"#fff\",colors:[],padding:6,layoutDirection:\"vertical\",borderColor:\"#fff\",borderWidth:0,handleRadius:8,activeHandleRadius:null,handleSvg:null,handleProps:{x:0,y:0},wheelLightness:!0,wheelAngle:0,wheelDirection:\"anticlockwise\",sliderSize:null,sliderMargin:12,boxHeight:null},{colors:[],display:\"block\",id:null,layout:\"default\",margin:null});var mt,xt,jt,Mt,Ot=(It.prototype=(mt=kt).prototype,Object.assign(It,mt),It.I=mt,It);function It(n,t){var i,r=document.createElement(\"div\");function e(){var t=n instanceof Element?n:document.querySelector(n);t.appendChild(i.base),i.onMount(t)}return function(t,n,i){var r,e,u;m.i&&m.i(t,n),e=(r=i===o)?null:i&&i.n||n.n,t=h(O,null,[t]),u=[],k(n,r?n.n=t:(i||n).n=t,e||x,x,void 0!==n.ownerSVGElement,i&&!r?[i]:e?null:j.slice.call(n.childNodes),u,!1,i||x,r),d(u,t)}(h(mt,Object.assign({},{ref:function(t){return i=t}},t)),r),\"loading\"!==document.readyState?e():document.addEventListener(\"DOMContentLoaded\",e),i}return(jt=xt=xt||{}).version=\"5.5.1\",jt.Color=V,jt.ColorPicker=Ot,(Mt=jt.ui||(jt.ui={})).h=h,Mt.ComponentBase=gt,Mt.Handle=bt,Mt.Slider=pt,Mt.Wheel=wt,Mt.Box=yt,xt});"
  "</script>";

/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (deviceMode == CONFIG_MODE && !isIp(server.hostHeader()) && server.hostHeader() != String(dns_name) + ".local") {
    Serial.print("[SERV] Request to ");
    Serial.print(server.hostHeader());
    Serial.println(" redirected to captive portal");
    server.sendHeader("Location", "http://" + String(dns_name) + ".local"/* + toStringIp(server.client().localIP())*/, true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

bool startMDNS() {
  int tryCount = 0;
  if (!MDNS.begin(dns_name, WiFi.softAPIP())) {
    Serial.println("[SERV] MDNS responder did not setup");
    while (tryCount < 10) {
      tryCount++;
      delay(500);
    }
    return false;
  }
  return true;
}

void startConfigPortal() {
  configStartMillis = millis(); // start counter

  updateColonColor(colorConfigMode);
  strip_show();

  String ap_name = AP_NAME + macLastThreeSegments(mac);
  IPAddress ap_ip(8, 8, 8, 8);

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_name.c_str());
  delay(100);
  Serial.print("[SERV] IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", ap_ip);

  startMDNS();
  startServer();

  Serial.println("[SERV] Captive portal started");

  unsigned int lastTest = 0;

  while (deviceMode == CONFIG_MODE) { // BLOCKING INFINITE LOOP

    time_t remainingSeconds = (CONFIG_TIMEOUT - (millis() - configStartMillis)) / 1000;

    if (millis() - configStartMillis > CONFIG_TIMEOUT) {
      WiFi.softAPdisconnect(true);
      //WiFi.mode(WIFI_STA);
      deviceMode = CONNECTION_FAIL;
      return;
    }

    if (millis() > lastTest + 100) {
      int splitTime[] = {

#ifndef CLOCK_VERSION_IV22
        (hour(remainingSeconds) / 10) % 10,
        hour(remainingSeconds) % 10,
#endif
        (minute(remainingSeconds) / 10) % 10,
        minute(remainingSeconds) % 10,
        (second(remainingSeconds) / 10) % 10,
        second(remainingSeconds) % 10,
      };

      for (int i = 0; i < registersCount; i++) {
#ifndef CLOCK_VERSION_IV22
        if (i < 4 && splitTime[i] == 0 && registersCount > 4) {
          blankDigit(i);
          continue;
        }
#endif
        setDigit(i, splitTime[i]);
      }
      lastTest = millis();
    }

    MDNS.update();
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
    delay(10);
  }
}

bool startServer() {
  httpUpdateServer.setup(&server, update_path, update_username, update_password);
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  //server.on("/json", handleJson); // show raw json at given url path

  // diyHue
  server.on("/detect", handleDiyHueDetect);
  server.on("/set", handleDiyHueSet);
  //server.on("/switch", handleDiyHueSwitch);
  server.on("/get", handleDiyHueGet);

  server.onNotFound(handleNotFound);
  server.begin();
}

void handleDiyHueDetect() {
  server.send(200, "text/plain", "{\"hue\": \"bulb\",\"lights\": " + String(1) + ",\"name\": \"" + AP_NAME + macLastThreeSegments(mac) +  "\",\"modelid\": \"LWB010\",\"mac\": \"" + String(mac[5], HEX) + ":"  + String(mac[4], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[0], HEX) + "\"}");
}

void handleDiyHueGet() {
  int diyhueBri;
  if (bri == 0) diyhueBri = 0;
  if (bri == 1) diyhueBri = 128;
  if (bri == 2) diyhueBri = 255;
  String power_status = isPoweredOn ? "true" : "false";
  server.send(200, "text/plain", "{\"on\": " + power_status + ", \"bri\": " + String(diyhueBri) + "}");
}

void handleDiyHueSet() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "on") {
      if (server.arg(i) == "True" || server.arg(i) == "true") {
        isPoweredOn = true;
      }
      else {
        isPoweredOn = false;
      }
    }
    else if (server.argName(i) == "bri" || server.argName(i) == "bri_inc") {
      isPoweredOn = true;
      if (server.arg(i).toInt() != 0) {
        if (server.arg(i).toInt() < 85) bri = 0;
        else if (server.arg(i).toInt() < 170) bri = 1;
        else bri = 2;
      }
    }
    else if (server.argName(i) == "alert" && server.arg(i) == "select") {
      if (isPoweredOn) {
        bri = 0;
      } else {
        bri = 2;
      }
    }
  }

  json["bri"] = bri;
  resetColonColor();
  showTime(); //

  /*
    int diyhueBri;
    if (bri == 0) diyhueBri = 0;
    if (bri == 1) diyhueBri = 128;
    if (bri == 2) diyhueBri = 255;
  */
  String power_status = isPoweredOn ? "true" : "false";
  server.send(200, "text/plain", "OK, bri:" + String(bri) + ", state:" + power_status);
}


void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }

  String html = FPSTR(html_header);
  html += "<h1>404 - Page not found</h1>";
  html += "<h2>\"The right man in the wrong place can make all the difference in the world.\"</h2>";
  for (uint8_t i = 0; i < server.args(); i++) {
    html += "<p>";
    html += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
    html += "</p>";
  }
  html += FPSTR(html_footer);

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  server.send(404, "text/html", html);
}
/*
  // Use only for testing => reveals wifi password!
  void handleJson() {
  String html;
  serializeJson(json, html);

  server.send(200, "text/json", html);

  }
*/
void handleRoot() {
  unsigned long serverProcessingTime = millis();

  if (server.args()) {

    if (server.hasArg("is_form")) {
      updateColonColor(colorConfigSave);
      strip_show();
    }

    if (server.hasArg("ssid")) {
      json["ssid"] = server.arg("ssid");
    }
    if (server.hasArg("pass") && server.arg("pass") == "placeholder") {
      Serial.println("password is a placeholder, skipped!");
    }
    if (server.hasArg("pass") && server.arg("pass") != "placeholder") {
      json["pass"] = server.arg("pass");
    }
    /*
      if (server.hasArg("ip")) {
      json["ip"] = server.arg("ip");
      }
      if (server.hasArg("gw")) {
      json["gw"] = server.arg("gw");
      }
      if (server.hasArg("sn")) {
      json["sn"] = server.arg("sn");
      }
    */
    if (server.hasArg("t_format")) {
      json["t_format"] = server.arg("t_format");
    }
    if (server.hasArg("zero")) {
      json["zero"] = server.arg("zero");
    }
    if (server.hasArg("on")) {
      isPoweredOn = server.arg("on").toInt();
    }
    if (server.hasArg("bri")) {
      json["bri"] = server.arg("bri");
      bri = json["bri"].as<int>();
      setupPhaseShift();
      initRgbColon(); // reinit colon color according to target bri
    }
    if (server.hasArg("fade")) {
      json["fade"] = server.arg("fade");
      crossFadeTime = json["fade"].as<int>();
      //setupCrossFade();
    }
    if (server.hasArg("colon")) {
      json["colon"] = server.arg("colon");
    }
    if (server.hasArg("rgb_h_r")) json["rgb"]["high"][0] = server.arg("rgb_h_r");
    if (server.hasArg("rgb_h_g")) json["rgb"]["high"][1] = server.arg("rgb_h_g");
    if (server.hasArg("rgb_h_b")) json["rgb"]["high"][2] = server.arg("rgb_h_b");
    if (server.hasArg("rgb_l_r")) json["rgb"]["low"][0] = server.arg("rgb_l_r");
    if (server.hasArg("rgb_l_g")) json["rgb"]["low"][1] = server.arg("rgb_l_g");
    if (server.hasArg("rgb_l_r")) json["rgb"]["low"][2] = server.arg("rgb_l_b");
    if (server.hasArg("rgb_en")) {
      json["rgb"]["en"] = 1;
      initRgbColon();
    } else {
      json["rgb"]["en"] = 0;
    }
    if (server.hasArg("nmode")) {
      json["nmode"] = server.arg("nmode");
    }
    if (server.hasArg("cathode")) {
      json["cathode"] = server.arg("cathode");
    }
    if (server.hasArg("rst_cycle")) {
      json["rst_cycle"] = server.arg("rst_cycle");
    }
    if (server.hasArg("rst_ip")) {
      json["rst_ip"] = server.arg("rst_ip");
    }
    if (server.hasArg("bal_enable")) {
      json["bal_enable"] = server.arg("bal_enable");
    } else {
      json["bal_enable"] = 0;
    }
    for (int i = 0; i < registersCount; i++) {
      if (server.hasArg("bal_h_" + String(i))) {
        json["bal"]["high"][i] = server.arg("bal_h_" + String(i));
      }
      if (server.hasArg("bal_l_" + String(i))) {
        json["bal"]["low"][i] = server.arg("bal_l_" + String(i));
      }
    }
    if (server.hasArg("ntp")) {
      json["ntp"] = server.arg("ntp");
    }
    if (server.hasArg("std_offset")) {
      json["std_offset"] = server.arg("std_offset");
    }
    if (server.hasArg("dst_enable")) {
      json["dst_enable"] = server.arg("dst_enable");
    } else {
      json["dst_enable"] = 0;
    }
    if (server.hasArg("std_week")) {
      json["std_week"] = server.arg("std_week");
    }
    if (server.hasArg("std_day")) {
      json["std_day"] = server.arg("std_day");
    }
    if (server.hasArg("std_month")) {
      json["std_month"] = server.arg("std_month");
    }
    if (server.hasArg("std_hour")) {
      json["std_hour"] = server.arg("std_hour");
    }
    if (server.hasArg("dst_offset")) {
      json["dst_offset"] = server.arg("dst_offset");
    }
    if (server.hasArg("dst_week")) {
      json["dst_week"] = server.arg("dst_week");
    }
    if (server.hasArg("dst_day")) {
      json["dst_day"] = server.arg("dst_day");
    }
    if (server.hasArg("dst_month")) {
      json["dst_month"] = server.arg("dst_month");
    }
    if (server.hasArg("dst_hour")) {
      json["dst_hour"] = server.arg("dst_hour");
    }
    saveConfig();

  }



  String html = "";

  if (!server.args() || (server.args() && server.hasArg("is_form"))) {
    html += FPSTR(html_header);
    html += FPSTR(html_wrapper_start);
    html += "<form method=\"post\" action=\"/\">";
    html += "<h1>FLORA - Wi-Fi VFD Clock</h1> <p></p>";

    if (deviceMode == CONFIG_MODE) {
      html += "<p>Config AP will automatically shut down after 5 minutes.</p> <p>After you save the settings and connect to wifi, you can open the config portal again using Clock's local IP address.</p>";
    }

    /* *** */
    /* WIFI SETTINGS */
    /* *** */

    html += "<h2>Network settings</h2> <p>Select your network settings here.</p> <div class=\"row\"> <label for=\"ssid\">WiFi SSID</label> <input type=\"text\" id=\"ssid\" name=\"ssid\" value=\"";
    html += json["ssid"].as<const char*>();
    html += "\"> </div> <div class=\"row\"> <label for=\"pass\">WiFi Password</label> <input type=\"password\" id=\"pass\" name=\"pass\" value=\"";
    const char* pass = json["pass"].as<const char*>();
    if (pass != NULL && pass[0] != '\0') {
      html += "placeholder";
    }
    /*
        html += "\"> </div> <h2>Static IP settings (optional)</h2> <p>Optional settings for static IP, in some cases this might speed up response time. All 3 need to be set and IP should be reserved in router's DHCP settings.";
        html += "<br>MAC address: <span class=\"mac\">";
        html += macToStr(mac);
        html += "</span></p>";
        html += "<div class=\"row\"> <label for=\"ip\">IP Address (optional):</label> <input type=\"text\" id=\"ip\" name=\"ip\" value=\"";
        html += json["ip"].as<const char*>();
        html += "\"> </div> <div class=\"row\"> <label for=\"gw\">Gateway IP (optional):</label> <input type=\"text\" id=\"gw\" name=\"gw\" value=\"";
        html += json["gw"].as<const char*>();
        html += "\"> </div> <div class=\"row\"> <label for=\"sn\">Subnet mask (optional):</label> <input type=\"text\" id=\"sn\" name=\"sn\" value=\"";
        html += json["sn"].as<const char*>();
    */
    html += "\"> </div>";

    /* *** */
    /* BASIC SETTINGS */
    /* *** */

    html += "<h2>Basic settings</h2><p>Time format and other useful settings and tweaks.</p>";

    html += "<div class=\"row\"><label for=\"t_format\">Time format:</label>";
    html += "<select id=\"t_format\" name=\"t_format\">";
    html += "<option value=\"1\"";
    if (json["t_format"].as<int>() == 1) html += " selected";
    html += ">24-hour</option>";
    html += "<option value=\"0\"";
    if (!json["t_format"].isNull() && json["t_format"].as<int>() == 0) html += " selected";
    html += ">12-hour</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"zero\">Leading zero:</label>";
    html += "<select id=\"zero\" name=\"zero\">";
    html += "<option value=\"1\"";
    if (json["zero"].as<int>() == 1) html += " selected";
    html += ">Show</option>";
    html += "<option value=\"0\"";
    if (!json["zero"].isNull() && json["zero"].as<int>() == 0) html += " selected";
    html += ">Hide</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"rst_cycle\">Cycle through digits after every reset:</label>";
    html += "<select id=\"rst_cycle\" name=\"rst_cycle\">";
    html += "<option value=\"0\"";
    if (json["rst_cycle"].as<int>() == 0) html += " selected";
    html += ">No</option>";
    html += "<option value=\"1\"";
    if (json["rst_cycle"].as<int>() == 1) html += " selected";
    html += ">Yes (good for testing all digits)</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"rst_ip\">Show IP address after reset (last 3 digits):</label>";
    html += "<select id=\"rst_ip\" name=\"rst_ip\">";
    int rst_ip = json["rst_ip"].as<int>();
    html += "<option value=\"1\"";
    if (rst_ip == 1) html += " selected";
    html += ">Yes</option>";
    html += "<option value=\"0\"";
    if (!json["rst_ip"].isNull() && rst_ip == 0) html += " selected";
    html += ">No</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"ntp\">NTP server address or IP</label>";
    html += "<input type=\"text\" id=\"ntp\" name=\"ntp\" placeholder=\"";
    html += ntpServerName;
    html += "\" value=\"";
    html += json["ntp"].as<const char*>();
    html += "\">";
    html += "<p>Leave empty to use ";
    html += ntpServerName;
    html += "</p>";
    html += "</div>";


    /* *** */
    /* VISUAL SETTINGS */
    /* *** */


    html += "<h2>Visual settings</h2><p>Brightness, animations and other fun stuff.</p>";


    html += "<div class=\"row\"><label for=\"bri\">Brightness:</label>";
    html += "<select id=\"bri\" name=\"bri\">";
    html += "<option value=\"2\"";
    if (json["bri"].as<int>() == 2) html += " selected";
    html += ">High</option>";
    html += "<option value=\"1\"";
    if (json["bri"].as<int>() == 1) html += " selected";
    html += ">Medium</option>";
    html += "<option value=\"0\"";
    if (!json["bri"].isNull() && json["bri"].as<int>() == 0) html += " selected";
    html += ">Low</option>";
    html += "</select></div>";


    html += "<div class=\"row\"><label for=\"bal_enable\">";
    html += "<input type=\"checkbox\" id=\"bal_enable\" name=\"bal_enable\" class=\"visibility-toggle\" data-target=\"bal_wrapper\" value=\"1\"";
    if (json["bal_enable"].as<int>() == 1) {
      html += " checked=\"checked\"";
    }
    html += ">&nbsp;Enable brightness balancing</label></div>";

    html += "<div id=\"bal_wrapper\">"; // Toggle wrapper start
    html += "<p>When buying used tubes, some are more severely burned out than others. Use options below to lower brightness of the good tubes to match the burned-out ones.</p>";


    html += "<h3>Values for high brightness setting:</h3>";
    html += "<div class=\"flexrow\">"; // FLEX ROW START

    for (int i = 0; i < registersCount; i++) {
      html += "<div class=\"col\">"; // COLUMN START
      html += "<div class=\"row\"><label for=\"bal_h_" + String(i) + "\">Digit " + String(i + 1) + ":</label>";
      html += "<select id=\"bal_h_" + String(i) + "\" name=\"bal_h_" + String(i) + "\">";

      for (int ii = pwmResolution; ii > 0; ii -= dimmingSteps) {
        html += "<option value=\"" + String(ii) + "\"";
        if ((!json["bal"]["high"][i].isNull() && json["bal"]["high"][i].as<int>() == ii) || (json["bal"]["low"][i].isNull() && ii == pwmResolution)) html += " selected";
        html += ">" + String(ii) + "</option>";
      }

      html += "</select></div>";
      html += "</div>"; // COLUMN END
    }
    html += "</div>"; // FLEX ROW END

    html += "<h3>Values for low brightness setting:</h3>";
    html += "<div class=\"flexrow\">"; // FLEX ROW START
    for (int i = 0; i < registersCount; i++) {
      html += "<div class=\"col\">"; // COLUMN START
      html += "<div class=\"row\"><label for=\"bal_l_" + String(i) + "\">Digit " + String(i + 1) + ":</label>";
      html += "<select id=\"bal_l_" + String(i) + "\" name=\"bal_l_" + String(i) + "\">";

      for (int ii = pwmResolution; ii > 0; ii -= dimmingSteps) {
        html += "<option value=\"" + String(ii) + "\"";
        if ((!json["bal"]["low"][i].isNull() && json["bal"]["low"][i].as<int>() == ii) || (json["bal"]["low"][i].isNull() && ii == dimmingSteps)) html += " selected";
        html += ">" + String(ii) + "</option>";
      }

      html += "</select></div>";
      html += "</div>"; // COLUMN END
    }
    html += "</div>"; // FLEX ROW END

    html += "</div>"; // Toggle wrapper end


    html += "<div class=\"row\"><label for=\"fade\">Crossfade animation:</label>";
    html += "<select id=\"fade\" name=\"fade\">";
    html += "<option value=\"30\"";
    if (json["fade"].as<int>() == 30) html += " selected";
    html += ">Slow</option>";
    html += "<option value=\"20\"";
    if (json["fade"].as<int>() == 20) html += " selected";
    html += ">Medium</option>";
    html += "<option value=\"15\"";
    if (json["fade"].as<int>() == 15) html += " selected";
    html += ">Fast</option>";
    html += "<option value=\"0\"";
    if (!json["fade"].isNull() && json["fade"].as<int>() == 0) html += " selected";
    html += ">Disable</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"nmode\">Night mode:</label>";
    html += "<select id=\"nmode\" name=\"nmode\">";
    unsigned int nmode = json["nmode"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (nmode == 0) html += " selected";
    html += ">None</option>";
    html += "<option value=\"1\"";
    if (nmode == 1) html += " selected";
    html += ">Set low brightness between 22:00-06:00</option>";
    //html += "<option value=\"2\"";
    //if (nmode == 2) html += " selected";
    //html += ">Clock OFF between 22:00-06:00</option>";
    html += "</select></div>";


    html += "<div class=\"row\"><label for=\"colon\">Colon:</label>";
    html += "<select id=\"colon\" name=\"colon\">";
    html += "<option value=\"2\"";
    if (json["colon"].as<int>() == 2) html += " selected";
    html += ">ON/OFF each second</option>";
    html += "<option value=\"0\"";
    if (!json["colon"].isNull() && json["colon"].as<int>() == 0) html += " selected";
    html += ">Off</option>";
    html += "<option value=\"1\"";
    if (json["colon"].as<int>() == 1) html += " selected";
    html += ">Always ON</option>";
#if !defined(CLOCK_VERSION_IV22)
    // gradient doesn't make sense on IV-22 colon
    html += "<option value=\"3\"";
    if (json["colon"].as<int>() == 3) html += " selected";
    html += ">Always ON (gradient)</option>";
#endif
    html += "</select></div>";


    html += "<div class=\"row\"><label for=\"rgb_en\">";
    html += "<input type=\"checkbox\" id=\"rgb_en\" name=\"rgb_en\" class=\"visibility-toggle\" data-target=\"rgb_wrapper\" value=\"1\"";
    if (json["rgb"]["en"].as<int>() == 1) {
      html += " checked=\"checked\"";
    }
    html += ">&nbsp;Enable colon color settings</label></div>";

    html += "<div id=\"rgb_wrapper\">"; // Toggle wrapper start

    html += "<p>This allows you to change default colon color to whatever you want. It's good for adjusting different WS2812 clones to match the VFDs. Btw, you can adjust the color live, without need to reset.</p>";

    html += "<div class=\"flexrow\">"; // FLEX ROW START

    html += "<div class=\"col col-xs-full\" style=\"display:flex;flex-direction:column;align-items:center;justify-content:flex-start;\">"; // COLUMN START
    html += "<h3 style=\"font-size: 16px;\">High brightness color:</h3>";
    html += "<div class=\"colorpicker\" id=\"picker_high\">";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_h_r\" name=\"rgb_h_r\" value=\"" + String(json["rgb"]["high"][0].as<int>()) + "\"></div>";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_h_g\" name=\"rgb_h_g\" value=\"" + String(json["rgb"]["high"][1].as<int>()) + "\"></div>";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_h_b\" name=\"rgb_h_b\" value=\"" + String(json["rgb"]["high"][2].as<int>()) + "\"></div>";
    html += "</div>";

    html += "</div>"; // COLUMN END

    html += "<div class=\"col col-xs-full\" style=\"display:flex;flex-direction:column;align-items:center;justify-content:flex-start;\">"; // COLUMN START
    html += "<h3 style=\"font-size: 16px;\">Low brightness color:</h3>";
    html += "<div class=\"colorpicker\" id=\"picker_low\">";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_l_r\" name=\"rgb_l_r\" value=\"" + String(json["rgb"]["low"][0].as<int>()) + "\"></div>";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_l_g\" name=\"rgb_l_g\" value=\"" + String(json["rgb"]["low"][1].as<int>()) + "\"></div>";
    html += "<div class=\"row\"><input type=\"hidden\" id=\"rgb_l_b\" name=\"rgb_l_b\" value=\"" + String(json["rgb"]["low"][2].as<int>()) + "\"></div>";
    html += "</div>"; // COLUMN END

    html += "</div>"; // FLEX ROW END





    html += "</div>";

    html += "</div>"; // Toggle wrapper end


    /* *** */
    /* TIMEZONE SETTINGS */
    /* *** */


    html += "<h2>Timezone settings</h2>";
    html += "<p>Set your timezone and optionally also daylight saving.</p>";


    html += "<div class=\"row\"><label for=\"std_offset\">Standard time UTC offset (in minutes, -660 = -11h, 660 = +11h):</label>";
    html += "<input type=\"number\" id=\"std_offset\" name=\"std_offset\" min=\"-660\" max=\"660\" value=\"";
    html += json["std_offset"].as<const char*>();
    html += "\"></div>";

    html += "<div class=\"row\"><label for=\"dst_enable\">";
    html += "<input type=\"checkbox\" id=\"dst_enable\" name=\"dst_enable\" class=\"visibility-toggle\" data-target=\"dst_wrapper\" value=\"1\"";
    if (json["dst_enable"].as<int>() == 1) {
      html += " checked=\"checked\"";
    }
    html += ">&nbsp;Enable Daylight Saving (summer time)</label></div>";
    html += "<p>When enabled, additional fields will pop up for you to set daylight saving time. <a href=\"https://www.timeanddate.com/time/map/\" target=\"_blank\">You can search for your timezone here</a></p>";



    html += "<div id=\"dst_wrapper\">"; // WRAPPER START

    html += "<h3>Standard (winter) time starts at:</h3>";

    html += "<div class=\"flexrow\">"; // FLEX ROW START

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_week\">Week:</label>";
    html += "<select id=\"std_week\" name=\"std_week\">";
    unsigned int std_week = json["std_week"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (std_week == 0) html += " selected";
    html += ">Last</option>";
    html += "<option value=\"1\"";
    if (std_week == 1) html += " selected";
    html += ">First</option>";
    html += "<option value=\"2\"";
    if (std_week == 2) html += " selected";
    html += ">Second</option>";
    html += "<option value=\"3\"";
    if (std_week == 3) html += " selected";
    html += ">Third</option>";
    html += "<option value=\"4\"";
    if (std_week == 4) html += " selected";
    html += ">Fourth</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_day\">Day:</label>";
    html += "<select id=\"std_day\" name=\"std_day\">";
    unsigned int std_day = json["std_day"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (std_day == 1) html += " selected";
    html += ">Sun</option>";
    html += "<option value=\"2\"";
    if (std_day == 2) html += " selected";
    html += ">Mon</option>";
    html += "<option value=\"3\"";
    if (std_day == 3) html += " selected";
    html += ">Tue</option>";
    html += "<option value=\"4\"";
    if (std_day == 4) html += " selected";
    html += ">Wed</option>";
    html += "<option value=\"5\"";
    if (std_day == 5) html += " selected";
    html += ">Thu</option>";
    html += "<option value=\"6\"";
    if (std_day == 6) html += " selected";
    html += ">Fri</option>";
    html += "<option value=\"7\"";
    if (std_day == 7) html += " selected";
    html += ">Sat</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_month\">Month:</label>";
    html += "<select id=\"std_month\" name=\"std_month\">";
    unsigned int std_month = json["std_month"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (std_month == 1) html += " selected";
    html += ">Jan</option>";
    html += "<option value=\"2\"";
    if (std_month == 2) html += " selected";
    html += ">Feb</option>";
    html += "<option value=\"3\"";
    if (std_month == 3) html += " selected";
    html += ">Mar</option>";
    html += "<option value=\"4\"";
    if (std_month == 4) html += " selected";
    html += ">Apr</option>";
    html += "<option value=\"5\"";
    if (std_month == 5) html += " selected";
    html += ">May</option>";
    html += "<option value=\"6\"";
    if (std_month == 6) html += " selected";
    html += ">Jun</option>";
    html += "<option value=\"7\"";
    if (std_month == 7) html += " selected";
    html += ">Jul</option>";
    html += "<option value=\"8\"";
    if (std_month == 8) html += " selected";
    html += ">Aug</option>";
    html += "<option value=\"9\"";
    if (std_month == 9) html += " selected";
    html += ">Sep</option>";
    html += "<option value=\"10\"";
    if (std_month == 10) html += " selected";
    html += ">Oct</option>";
    html += "<option value=\"11\"";
    if (std_month == 11) html += " selected";
    html += ">Nov</option>";
    html += "<option value=\"12\"";
    if (std_month == 12) html += " selected";
    html += ">Dec</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_hour\">Hour:</label>";
    html += "<input type=\"number\" id=\"std_hour\" name=\"std_hour\" min=\"0\" max=\"23\" value=\"";
    html += json["std_hour"].as<const char*>();
    html += "\"></div>";
    html += "</div>"; // COLUMN END

    html += "</div>"; // FLEX ROW END







    html += "<h3>Daylight saving (summer) time starts at:</h3>";

    html += "<div class=\"flexrow\">"; // FLEX ROW START

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_week\">Week:</label>";
    html += "<select id=\"dst_week\" name=\"dst_week\">";
    unsigned int dst_week = json["dst_week"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (dst_week == 0) html += " selected";
    html += ">Last</option>";
    html += "<option value=\"1\"";
    if (dst_week == 1) html += " selected";
    html += ">First</option>";
    html += "<option value=\"2\"";
    if (dst_week == 2) html += " selected";
    html += ">Second</option>";
    html += "<option value=\"3\"";
    if (dst_week == 3) html += " selected";
    html += ">Third</option>";
    html += "<option value=\"4\"";
    if (dst_week == 4) html += " selected";
    html += ">Fourth</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_day\">Day:</label>";
    html += "<select id=\"dst_day\" name=\"dst_day\">";
    unsigned int dst_day = json["dst_day"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (dst_day == 1) html += " selected";
    html += ">Sun</option>";
    html += "<option value=\"2\"";
    if (dst_day == 2) html += " selected";
    html += ">Mon</option>";
    html += "<option value=\"3\"";
    if (dst_day == 3) html += " selected";
    html += ">Tue</option>";
    html += "<option value=\"4\"";
    if (dst_day == 4) html += " selected";
    html += ">Wed</option>";
    html += "<option value=\"5\"";
    if (dst_day == 5) html += " selected";
    html += ">Thu</option>";
    html += "<option value=\"6\"";
    if (dst_day == 6) html += " selected";
    html += ">Fri</option>";
    html += "<option value=\"7\"";
    if (dst_day == 7) html += " selected";
    html += ">Sat</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_month\">Month:</label>";
    html += "<select id=\"dst_month\" name=\"dst_month\">";
    unsigned int dst_month = json["dst_month"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (dst_month == 1) html += " selected";
    html += ">Jan</option>";
    html += "<option value=\"2\"";
    if (dst_month == 2) html += " selected";
    html += ">Feb</option>";
    html += "<option value=\"3\"";
    if (dst_month == 3) html += " selected";
    html += ">Mar</option>";
    html += "<option value=\"4\"";
    if (dst_month == 4) html += " selected";
    html += ">Apr</option>";
    html += "<option value=\"5\"";
    if (dst_month == 5) html += " selected";
    html += ">May</option>";
    html += "<option value=\"6\"";
    if (dst_month == 6) html += " selected";
    html += ">Jun</option>";
    html += "<option value=\"7\"";
    if (dst_month == 7) html += " selected";
    html += ">Jul</option>";
    html += "<option value=\"8\"";
    if (dst_month == 8) html += " selected";
    html += ">Aug</option>";
    html += "<option value=\"9\"";
    if (dst_month == 9) html += " selected";
    html += ">Sep</option>";
    html += "<option value=\"10\"";
    if (dst_month == 10) html += " selected";
    html += ">Oct</option>";
    html += "<option value=\"11\"";
    if (dst_month == 11) html += " selected";
    html += ">Nov</option>";
    html += "<option value=\"12\"";
    if (dst_month == 12) html += " selected";
    html += ">Dec</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_hour\">Hour:</label>";
    html += "<input type=\"number\" id=\"dst_hour\" name=\"dst_hour\" min=\"0\" max=\"23\" value=\"";
    html += json["dst_hour"].as<const char*>();
    html += "\"></div>";
    html += "</div>"; // COLUMN END

    html += "</div>"; // FLEX ROW END

    html += "<div class=\"row\"><label for=\"dst_offset\">Daylight saving (summer) time UTC offset (in minutes):</label>";
    html += "<input type=\"number\" id=\"dst_offset\" name=\"dst_offset\" min=\"-660\" max=\"660\" value=\"";
    html += json["dst_offset"].as<const char*>();
    html += "\"></div>";

    html += "</div>"; // WRAPPER END

    // This is just to check if it's a remote GET request or full update.
    html += "<input type=\"hidden\" name=\"is_form\" value=\"1\">";

    html += "<button type=\"submit\">Save and reboot</button></form>";
    //html += FPSTR(html_wrapper_end);

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", html);

    html = FPSTR(html_wrapper_end);
    html.replace("{{fw_name}}", FW_NAME);
    html.replace("{{fw_version}}", FW_VERSION);
    server.sendContent(html);

    server.sendContent(iro);

    html = FPSTR(html_footer);
    html.replace("{{rgb_high}}", "rgb(" + json["rgb"]["high"][0].as<String>() + "," + json["rgb"]["high"][1].as<String>() + "," + json["rgb"]["high"][2].as<String>() + ")");
    html.replace("{{rgb_low}}", "rgb(" + json["rgb"]["low"][0].as<String>() + "," + json["rgb"]["low"][1].as<String>() + "," + json["rgb"]["low"][2].as<String>() + ")");
    server.sendContent(html);

    server.sendContent(""); // this tells web client that transfer is done

  } else {
    for (uint8_t i = 0; i < server.args(); i++) {
      html += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(200, "text/plain", html);
  }

  Serial.print("[SERV] Serving HTML took: ");
  Serial.print(millis() - serverProcessingTime);
  Serial.println(" ms");

  if (server.args() && server.hasArg("is_form")) {
    delay(1000);
    ESP.restart();
    delay(100);
  }

}
