
    // Load library.
    google.load('visualization', '1', {packages: ['corechart']});
$(document).ready(function() {
  // If data is provide.
  if (typeof Drupal.settings.gvapi != 'undefined') {
    // Rows data.
    var rows = Drupal.settings.gvapi.rows;
    // Column data.
    var columns = Drupal.settings.gvapi.columns;
    // Draw callback function.
    function draw() {
      var DT = new google.visualization.DataTable();
      for (i in columns) {
        DT.addColumn(columns[i].key, columns[i].value);
      }
      for (i in rows) {
        DT.addRow(rows[i]);
      }
      switch (Drupal.settings.gvapi.type) {
        case 'LineChart':
          new google.visualization.LineChart(document.getElementById('chart')).draw(DT, Drupal.settings.gvapi.config);
        break;
      }
    }
    google.setOnLoadCallback(draw);
  }
});
