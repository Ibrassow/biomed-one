#include <IotWebConf.h>





String init_page = R"=====(
<!DOCTYPE html>
<html>
<div style="text-align: center;">
  <body>
    <h1>Bienvenue</h1>
    <p>Veuillez d&eacute;buter la configuration du dispositif en cliquant sur "COMMENCER".</p>
    <form action="/param">
      <input type="submit" name="Submit" value="Commencer" /> </form>
  </body>
</div>
</html>

)=====";


String root_page = R"=====(
<html>
<div style="text-align: center;">
  <body>
    <h1>Bienvenue</h1>
    <p>Vous vous trouvez sur la page web du dispositif. Choisissez votre option.</p>
    <form action="/param">
      <input type="submit" name="Submit" value="Modifier vos donn&eacute;es personnelles" /> </form>
    <br>
    <form action="/config">
      <input type="submit" name="Submit" value="Modifier vos param&egrave;tres WiFi" /> </form>
  </body>
</div>
</html>

)=====";





String redirect_page = R"=====(<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="refresh" content="2; URL=/config" />
  </head>
<body>
<center>
<h1>Vous allez être redirigez vers la page de configuration du wifi dans quelques secondes</h1>
<p>Si vous n'avez toujours pas &eacute;t&eacute; r&eacute;dirig&eacute; au bout de quelques secondes, cliquez sur ce <a href='config'>lien</a>.
</p>
</center>
</body>
</html>)=====";





String param_page = R"=====(<!DOCTYPE HTML>
<html> ​

<head>
  <div style="text-align: center;">
    <h1>Service de surveillance m&eacute;dical</h1>
    <meta name="viewport" content="width=device-width, initial-scale=1"> </head>
<p>Nous vous remercions pour votre confiance. </p>
<p>Notre service de surveillance m&eacute;dicale garantit la s&eacute;curit&eacute;, la pr&eacute;cision et l'int&eacute;grit&eacute; de votre condition en combinant r&eacute;activit&eacute;, personnel qualifi&eacute; et technologie avanc&eacute;e. Chez BiomedOne, votre sant&eacute; est la priorit&eacute;. </p>
<p>Avant de d&eacute;buter, nous avons besoin d'informations vous concernant afin d'adapter notre service &agrave; votre cas particulier. </p> ​

<body>
  <fieldset>
    <legend>Informations personnelles:</legend>
    <form action="/wait">
      <p style="margin-bottom:0.5cm;">
        <label for="name">Nom:</label>
        <input type="text" name="name" required> </p>
      <p style="margin-bottom:0.5cm;">
        <label for="surname">Pr&eacute;nom:</label>
        <input type="text" name="surname" required> </p>
      <p style="margin-bottom:0.5cm;">
        <label for="age">&Agrave;ge:</label>
        <input type="text" name="age" required> </p>
      <p style="margin-bottom:0.5cm;">
        <label for="gender">Genre:</label>
        <input type="radio" id="male" name="gender" value="male" required>
        <label for="male">Homme</label>
        <input type="radio" id="female" name="gender" value="female">
        <label for="female">Femme</label>
      </p>
      <p style="margin-bottom:0.5cm;">
        <label for="maildress">Adresse mail:</label>
        <input type="text" name="mailadress" required> </p>
      <p style="margin-bottom:0.5cm;">
        <label for="risk">Auriez-vous un probl&egrave;me de sant&eacute; hors COVID-19 vous pr&eacute;sentant comme une personne &agrave; risque (probl&egrave;mes cardiaques et/ou respiratoires, maladies chroniques, etc.. ) ? </label>
        <select name="risk" required>
          <option disabled selected value> -- S&eacute;lectionner une option -- </option>
          <option value="False">Non</option>
          <option value="True">Oui</option>
        </select>
      </p>
      <p style="margin-bottom:0.5cm;">
        <label for="info_add">Autres informations que vous souhaiteriez nous communiquer:</label>
        <br>
        <textarea name="info_add" rows="3" cols="40"> </textarea>
        <p style="margin-bottom:0.5cm;"> Pour proc&eacute;der &agrave; la suite, veuillez cliquer sur envoyer. Vous serez redirig&eacute; sur une page vous permettant de configurer la connexion WiFi de votre dispositif. </p>
        <p style="margin-bottom:0.5cm;">
          <input type="submit" value="Envoyer"> </p></form>
  </fieldset> ​ ​ ​ 
  <img src="https://us.123rf.com/450wm/sylverarts/sylverarts1708/sylverarts170800653/83918909-stock-vector-vector-graphic-illustration-of-strong-male-depicted-as-dna-strands-continuation-and-created-with-wir.jpg?ver=6" </img>
  <p> INSERT TEXT GDPR </p>
  </div>)=====";
