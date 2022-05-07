var db = openDatabase('mydb', '1.0', 'Test DB', 2 * 1024 * 1024); 
var msg; 

db.transaction(function (tx) { 
   tx.executeSql('CREATE TABLE IF NOT EXISTS LOGS2 (id unique, log)'); 
   tx.executeSql('INSERT INTO LOGS2 (id, log) VALUES (1, "foobar")'); 
   tx.executeSql('INSERT INTO LOGS2 (id, log) VALUES (2, "logmsg")'); 
   msg = '<p>Log message created and row inserted.</p>'; 
   document.querySelector('#status').innerHTML =  msg; 
})

db.transaction(function (tx) { 
   tx.executeSql('SELECT * FROM LOGS2', [], function (tx, results) { 
      var len = results.rows.length, i; 
      msg = "<p>Found rows: " + len + "</p>"; 
      document.querySelector('#status').innerHTML +=  msg; 

      for (i = 0; i < len; i++) { 
         msg = "<p><b>" + results.rows.item(i).log + "</b></p>"; 
         document.querySelector('#status').innerHTML +=  msg; 
      } 
   }, null); 
}); 