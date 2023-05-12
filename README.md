Git instructies voor het netjes onderhouden van alle branches:

Check op veranderingen binnen je eigen branch:
	- git fetch

Indien er staat dat er veranderingen zijn, haal de veranderingen binnen:
	- git pull


Het navigeren naar een branch:
	- git checkout "branch_naam"

Het maken van een nieuwe branch:
	- git checkout -b "nieuwe_branch_naam"


Check eerst voor de zekerheid de status van jouw lokale branch:
	- git status
Alleen de files die jij hebt aangepast zouden rood moeten zijn.
Als er andere files ook rood zijn moet je even dubbelchecken of alles nog OK is.

Toevoegen van code aan de remote branch:
	- git add "naam_van_je_file"

Indien je alles wat bij de status check rood was toe wilt voegen:
	- git add *

Vervolgens commit je alle toegevoegde files met een korte beschrijving van wat het toe voegt:
	- git commit -m "jouw_korte_beschrijving_in_het_engels"

Als de commit door git geaccepteerd wordt kan je het pushen met:
	- git push

KIJK ALTIJD UIT DAT JE IN DE GOEDE BRANCH ZIT
Het is NIET mogelijk om te pushen in de masterbranch.
Zorg dat je altijd in een branch zit die relevant is met wat je aan het doen bent.
