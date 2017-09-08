<HTML>
  <HEAD>
    <link rel="stylesheet" href="./style.css" type="text/css" media="all" /><html>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" crossorigin="anonymous">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap-theme.min.css" crossorigin="anonymous">
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js" crossorigin="anonymous"></script>
    <TITLE>enlaces.php</TITLE>
  </HEAD>
  <BODY>
    <div id="contenedor">
      <div id="resultados">
        <?php
          error_reporting(E_ALL);
          ini_set('display_errors', '1');
          session_start();

          $v1=$_GET['m1'];
          $v2=$_GET['m2'];
          $v3=$_GET['m3'];
        ?>
        <h1><?php echo "Palabra clave: $v1 <br>";?></h1>
        <h2 id="dominio"><?php  echo "Dominio: $v2 <br>";?></h2>

        <?php
          $prueba = $v3;
          $array = explode("<br>",$prueba);
          $longitud = count($array);

          for ($i=0; $i < $longitud ; $i++) {
        ?>
            <a href="<?php echo "$array[$i]";?>"><?php echo "$array[$i]<br>";?></a> <br>
        <?php
          }
        ?>

        <input type="button" value="Volver Atrás" onclick="history.back(-1)" />
      </div>
      <div id="barra_lateral_2">
        <a href="https://www.ugr.es/"><img src="logo_ugr.png" alt="ugr" /></a>
        <a href="http://etsiit.ugr.es/"><img src="logo_etsiit.png" alt="ETSIIT" /></a>
        <a href="https://github.com/javiexfiliana7"><img src="github.png" alt="GitHub" /></a>
        <div id="reloj"> <iframe src="https://www.zeitverschiebung.net/clock-widget-iframe-v2?language=es&timezone=Europe%2FMadrid" width="100%" height="150" frameborder="0" seamless></iframe></div>
      </div>
    </div>
  </BODY>
</HTML>
