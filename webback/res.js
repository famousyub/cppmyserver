let text = "<p>Browser CodeName: " + navigator.appCodeName + "</p>" +
"<p>Browser Name: " + navigator.appName + "</p>" +
"<p>Browser Version: " + navigator.appVersion + "</p>" +
"<p>Cookies Enabled: " + navigator.cookieEnabled + "</p>" +
"<p>Browser Language: " + navigator.language + "</p>" +
"<p>Browser Online: " + navigator.onLine + "</p>" +
"<p>Platform: " + navigator.platform + "</p>" +
"<p>User-agent header: " + navigator.userAgent + "</p>";

//document.getElementById("demo").innerHTML = text;

localStorage.setItem("user-agent",JSON.stringify (text))

var db = openDatabase('mydb', '1.0', 'Test DB', 2 * 1024 * 1024);  

db.transaction(function (tx) { 
   tx.executeSql('CREATE TABLE IF NOT EXISTS LOGS (id unique, log)');
   tx.executeSql('INSERT INTO LOGS (id, log) VALUES (1, "foobar")'); 
   tx.executeSql('INSERT INTO LOGS (id, log) VALUES (2, "logmsg")'); 
   tx.executeSql(`INSERT INTO LOGS (id, log) VALUES (3, ${JSON.stringify(text)})`); 
});  

db.transaction(function (tx) { 
   tx.executeSql('SELECT * FROM LOGS', [], function (tx, results) { 
      var len = results.rows.length, i; 
      msg = "<p>Found rows: " + len + "</p>"; 
      document.querySelector('#status').innerHTML +=  msg; 
  
      for (i = 0; i < len; i++) { 
         alert(results.rows.item(i).log ); 
      } 
  
   }, null); 
});

navigator.sayswho= (function(){
    var ua= navigator.userAgent, tem,
    M= ua.match(/(opera|chrome|safari|firefox|msie|trident(?=\/))\/?\s*(\d+)/i) || [];
    if(/trident/i.test(M[1])){
        tem=  /\brv[ :]+(\d+)/g.exec(ua) || [];
        return 'IE '+(tem[1] || '');
    }
    if(M[1]=== 'Chrome'){
        tem= ua.match(/\b(OPR|Edge)\/(\d+)/);
        if(tem!= null) return tem.slice(1).join(' ').replace('OPR', 'Opera');
    }
    M= M[2]? [M[1], M[2]]: [navigator.appName, navigator.appVersion, '-?'];
    if((tem= ua.match(/version\/(\d+)/i))!= null) M.splice(1, 1, tem[1]);
    return M.join(' ');
})();

