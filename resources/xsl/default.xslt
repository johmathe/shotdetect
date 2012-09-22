<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:variable name="idContenu"><xsl:value-of select="/shotdetect/content/@id"/></xsl:variable>
	<xsl:variable name="enteteImage"><xsl:value-of select="/shotdetect/content/@id"/>_</xsl:variable>
	<xsl:variable name="dossierImage"><xsl:value-of select="/shotdetect/content/@id"/></xsl:variable>
	<xsl:variable name="title"><xsl:value-of select="/shotdetect/content/head/title"/></xsl:variable>
	<xsl:variable name="author"><xsl:value-of select="/shotdetect/content/head/author/name"/><xsl:text> </xsl:text><xsl:value-of select="/shotdetect/content/head/author/surname"/></xsl:variable>
	<xsl:variable name="date"><xsl:value-of select="/shotdetect/content/head/year"/></xsl:variable>
	<xsl:variable name="abstract"><xsl:value-of select="/shotdetect/content/head/abstract"/></xsl:variable>
		
	<xsl:variable name="srcVideo"><xsl:value-of select="/shotdetect/content/media/@src"/></xsl:variable>
	<xsl:variable name="dureeFilm"><xsl:value-of select="/shotdetect/content/media/duration"/></xsl:variable>

	<xsl:variable name="authorAnnotation">DDC - IRI</xsl:variable>
	<xsl:variable name="dateAnnotation">06/2006</xsl:variable>
	
<!--
/ : 
Appelle : SHOTDETECT
Appelé par : 
-->
	<xsl:template match="/">
			<xsl:apply-templates select="shotdetect"/>
	</xsl:template>	
	
<!--
SHOTDETECT : 
Appelle : CONTENT
Appelé par : 
-->
	<xsl:template match="shotdetect">
		<iri>
			<xsl:apply-templates select="content"/>
		</iri>
	</xsl:template>
	
<!--
HEAD :
Appelle :
Appelé par : 
-->
	<xsl:template match="head">
		<head>
				<meta name="id" content="{$idContenu}"/>
				<meta name="title" content="{$title}"/>
				<meta name="author" content="{$author}"/>
				<meta name="contributor" content=""/>
				<meta name="type" content="Film"/>
				<meta name="copyright" content="(c) ddc"/>
				<meta name="date" content="{$date}"/>
				<meta name="abstract" content="{$abstract}"/>
		</head>
	</xsl:template>
	
<!--
CONTENT : 
Appelle : CALCUL_DUREE_PLAN
Appelé par : 
-->
	<xsl:template match="content">
		<xsl:apply-templates select="head"/>
		<body>
			<ensembles>
			<!-- Image -->
					<ensemble>
						<xsl:attribute name="id">en_2</xsl:attribute>
						<xsl:attribute name="author"><xsl:value-of select="$authorAnnotation"/></xsl:attribute>
						<xsl:attribute name="date"><xsl:value-of select="$dateAnnotation"/></xsl:attribute>
						<xsl:attribute name="display">2</xsl:attribute>
						<xsl:attribute name="title">Image</xsl:attribute>
						<xsl:attribute name="abstract">Découpage visuel</xsl:attribute>
						
						<!-- Création du plan par plan de base-->
						<xsl:call-template name="segmentation_decoupage">
							<xsl:with-param name="name" select="'decoupage'"/>
							<xsl:with-param name="noeud" select="body/shots/shot"/>
							<xsl:with-param name="dureeFilm" select="$dureeFilm"/>
							<xsl:with-param name="id_seg" select="'PPP'"/>
							<xsl:with-param name="type" select="''"/>
							<xsl:with-param name="display" select="'1'"/>
							<xsl:with-param name="title" select="'Plan par plan'"/>
							<xsl:with-param name="label" select="'P.P.P.'"/>
							<xsl:with-param name="abstract" select="'Découpage par changement de plan'"/>
						</xsl:call-template>
					</ensemble>
				</ensembles>
			<!-- Liens -->
					<links/>
			<!-- Médias -->
					<medias>
						<media id="audio"/>
						<media id="image"/>
						<media id="text"/>
						<media id="video">
								<video id="a12" q="MQ" src="{$srcVideo}" begin="0">
									<xsl:attribute name="dur"><xsl:value-of select="$dureeFilm"/></xsl:attribute>
								</video>
						</media>
						<media id="tool"/>
					</medias>
			</body>
	</xsl:template>
	
	
	
<!--
SEGMENTATION_DECOUPAGE : Crée le découpage de base du plan par plan
Appelle : CALCUL_DUREE_PLAN
Appelé par : 
-->
	<xsl:template name="segmentation_decoupage">
		<xsl:param name="name"/>
		<xsl:param name="noeud"/>
		<xsl:param name="dureeFilm"/>
		<xsl:param name="id_seg"/>
		<xsl:param name="type"/>
		<xsl:param name="display"/>
		<xsl:param name="title"/>
		<xsl:param name="label"/>
		<xsl:param name="abstract"/>
		

			<xsl:element name="{$name}">
				<xsl:attribute name="id">
					<xsl:call-template name="cree_id">
						<xsl:with-param name="entete" select="'de'"/>
						<xsl:with-param name="chaine" select="$id_seg"/>
					</xsl:call-template>
				</xsl:attribute>

				<xsl:attribute name="dur"><xsl:value-of select="$dureeFilm"/></xsl:attribute>
				<xsl:attribute name="color"></xsl:attribute>
				<xsl:attribute name="author"><xsl:value-of select="$authorAnnotation"/></xsl:attribute>
				<xsl:attribute name="date"><xsl:value-of select="$dateAnnotation"/></xsl:attribute>
				<xsl:attribute name="display"><xsl:value-of select="$display"/></xsl:attribute>
				<xsl:attribute name="title"><xsl:value-of select="$title"/></xsl:attribute>
				<xsl:attribute name="label"><xsl:value-of select="$label"/></xsl:attribute>
				<xsl:attribute name="abstract"><xsl:value-of select="$abstract"/></xsl:attribute>
				<elements>
				
					<xsl:for-each select="$noeud">
							<element>
								<!-- Calcul du TC de début du segment   00:00:15:00 -->
								<xsl:variable name="inTC"><xsl:value-of select="@msbegin"/></xsl:variable>
								<!-- Calcul de la durée du segment   00:00:15:00 -->
								<xsl:variable name="outTC"><xsl:value-of select="@msduration"/></xsl:variable>
								
								<xsl:attribute name="id">
									<xsl:call-template name="cree_id">
										<xsl:with-param name="entete" select="'elD'"/>
										<xsl:with-param name="chaine" select="@id"/>
									</xsl:call-template>
								</xsl:attribute>
								<xsl:attribute name="order"/>
								<xsl:attribute name="prev"/>
								<xsl:attribute name="next"/>
								<xsl:attribute name="begin"><xsl:value-of select="$inTC"/></xsl:attribute>
								<xsl:attribute name="dur"><xsl:value-of select="$outTC"/></xsl:attribute>
								<xsl:attribute name="author"><xsl:value-of select="$authorAnnotation"/></xsl:attribute>
								<xsl:attribute name="date"><xsl:value-of select="$dateAnnotation"/></xsl:attribute>
								<xsl:attribute name="title">plan <xsl:value-of select="@id"/></xsl:attribute>
								<xsl:attribute name="abstract"></xsl:attribute>
								<!-- Images -->
								<xsl:variable name="pict"><xsl:value-of select="img[@size='thumb' and @type='in']/@src"/> </xsl:variable>
								<xsl:attribute name="src"><xsl:value-of select="substring-after($pict,'/')"/> </xsl:attribute>
								
							</element>
					</xsl:for-each>
				</elements>

				<!-- Création des vues -->
				<views/>
			</xsl:element>
	</xsl:template>
	
	
<!--
CREE_ID : Permet de créer in ID à partir d'une chaine de caractère et d'une entête
Appelle : TRANSFORME_CHAINE_CARACTERE_INTERDIT
Appelé par : 
-->
<xsl:template name="cree_id">
<xsl:param name="entete"/>
<xsl:param name="chaine"/>

	<xsl:variable name="chaine_temp">
		<xsl:call-template name="tranforme_chaine">
			<xsl:with-param name="chaine">
				<xsl:call-template name="tranforme_chaine_caractere_interdit">
					<xsl:with-param name="chaine" select="$chaine"/>
				</xsl:call-template>			
			</xsl:with-param>
		</xsl:call-template>	
	</xsl:variable>

	<xsl:value-of select="$entete"/>_<xsl:value-of select="$chaine_temp"/>
</xsl:template>	


<!--
TRANSFORME_CHAINE_CARACTERE_INTERDIT : Permet de supprimer les caractères interdits d'une chaine
(utiliser avec TRANSFORME_CHAINE pour créer un ID) !!!!!!!!!!!!!!!!!!!!!!!!!! MANQUE l'APOSTROPHE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-->
<xsl:template name="tranforme_chaine_caractere_interdit">
<xsl:param name="chaine"/>

    <!-- Variable représentant une apostrophe -->
	<xsl:variable name="apostrophe"><xsl:text>'</xsl:text></xsl:variable>
	<xsl:variable name="apostrophe2"><xsl:text>’</xsl:text></xsl:variable>
	
	<xsl:value-of select="translate(translate($chaine,'~#{([|`\^@])}=+$¤£%µ*§!:/;,.’? ','                          _'),$apostrophe,'')"/>
	
</xsl:template>
	
<!--
	TRANSFORME_CHAINE : Permet de transformer une chaine en majuscule 
	(utiliser avec TRANSFORME_CHAINE_CARACTERE_INTERDIT pour créer un ID)
	
	translate($ma-chaine, "ÀÁÂÃÄÅÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝàáâãäåçèéêëìíîïñòóôõöùúûüýÿ", "AAAAAACEEEEIIIINOOOOOUUUUYaaaaaaceeeeiiiinooooouuuuyy")
	
	minuscule = "aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzzaaaaaaaaaaaaææcceeeeeeeeiiiiiiiiððnnooooooooooøøuuuuuuuuyyþþ"
	majuscule = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÀàÁáÂâÃãÄäÅåÆæÇçÈèÉéÊêËëÌìÍíÎîÏïÐðÑñÒòÓóÔôÕõÖöØøÙùÚúÛûÜüÝýÞþ"
	translate($ma-chaine, $minuscule, $majuscule )
-->
	<xsl:template name="tranforme_chaine">
		<xsl:param name="chaine"/>
		<xsl:param name="type"/>
		<xsl:choose>
			<xsl:when test="$type = 'minuscule'">
				<xsl:value-of select="translate($chaine,'ABCDEFGHIJKLMNOPQRSTUVWXYZÈÉÊËÀÁÂÃÄÅÙÚÛÜÒÓÔÕÖÌÍÎÏÇÑÝ','abcdefghijklmnopqrstuvwxyzéèêëàáâãäåùúûüòóôõöìíîïçñýÿ')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="translate($chaine,'abcdefghijklmnopqrstuvwxyzéèêëÈÉÊËàáâãäåÀÁÂÃÄÅùúûüÙÚÛÜòóôõöÒÓÔÕÖìíîïÌÍÎÏçÇñÑýÿÝ','ABCDEFGHIJKLMNOPQRSTUVWXYZEEEEEEEEAAAAAAAAAAAAUUUUUUUUOOOOOOOOOOIIIIIIIICCNNYYY')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>	
	
	
</xsl:stylesheet>
