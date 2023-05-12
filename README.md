<html>
<body>
	<h1>Git instructies voor het netjes onderhouden van alle branches:</h1>
	<p>Check op veranderingen binnen je eigen branch:</p>
	<blockquote>
		<p>- git fetch</p>
	</blockquote>
	<p>Indien er staat dat er veranderingen zijn, haal de veranderingen binnen:</p>
	<blockquote>
		<p>- git pull</p>
	</blockquote>
	<p>Het navigeren naar een branch (zonder &lt;&lt; &gt;&gt;):</p>
	<blockquote>
		<p>- git checkout &lt;&lt;branch_naam&gt;&gt;</p>
	</blockquote>
	<p>Het maken van een nieuwe branch (zonder &lt;&lt; &gt;&gt;):</p>
	<blockquote>
		<p>- git checkout -b &lt;&lt;nieuwe_branch_naam&gt;&gt;</p>
	</blockquote>
	<p>Check eerst voor de zekerheid de status van jouw lokale branch:</p>
	<blockquote>
		<p>- git status</p>
	</blockquote>
	<p>Alleen de files die jij hebt aangepast zouden rood moeten zijn. Als er andere files ook rood zijn moet je even dubbelchecken of alles nog OK is.</p>
	<p>Toevoegen van code aan de remote branch (zonder &lt;&lt; &gt;&gt;):</p>
	<blockquote>
		<p>- git add &lt;&lt;naam_van_je_file&gt;&gt;</p>
	</blockquote>
	<p>Indien je alles wat bij de status check rood was toe wilt voegen:</p>
	<blockquote>
		<p>- git add *</p>
	</blockquote>
	<p>Vervolgens commit je alle toegevoegde files met een korte beschrijving van wat het toe voegt (inclusief aanhalingstekens):</p>
	<blockquote>
		<p>- git commit -m "jouw_korte_beschrijving_in_het_engels"</p>
	</blockquote>
	<p>Als de commit door git geaccepteerd wordt kan je het pushen met:</p>
	<blockquote>
		<p>- git push De eerste keer dat je naar een nieuwe branch pusht, moet je de local koppelen aan de remote branch.</p>
	</blockquote>
	<p>Dit gaat als volgt (zonder &lt;&lt; &gt;&gt;):</p>
	<blockquote>
		<p>- git push --set-upstream origin &lt;&lt;branchnaam&gt;&gt;</p>
	</blockquote>
	<p>KIJK ALTIJD UIT DAT JE IN DE GOEDE BRANCH ZIT</p>
	<p>Het is NIET mogelijk om te pushen in de masterbranch.</p>
	<p>Zorg dat je altijd in een branch zit die relevant is met wat je aan het doen bent.</p>
</body>
</html>