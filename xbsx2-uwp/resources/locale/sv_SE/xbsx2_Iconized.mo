��    L      |  e   �      p  �   q  �   �  2   �  �   �  �   �     5	  �   �	  `   |
  :   �
  :     :   S  B   �  H   �  4     Q   O  �   �  �   7  M     /   Y  M   �  �   �    �  ]   �  �   �  G   �  H   �    7  t   U  �   �  �   M    �  d   �  �   R  b   �  �   G  �     5   �  �   �  �   �  �   �  �   R  �     r  �  |   \  |   �  �   V  �  E   �   �!  �   �"  �   w#  J   $  �   [$  p   �$  G  a%  -   �&  �   �&  �   ['  �   �'  �   �(  �   Z)  �   �)  �   �*  �   �+  �   ?,  �   �,  *  �-  �   �.  �   �/  \   �0  r   �0  �   \1  �   �1  �   �2  �   �3  �  24  �  �5  �   F8  �   �8  P   �9    �9  �   �:  �   �;  �   N<  r   1=  =   �=  =   �=  =    >  C   ^>  F   �>  >   �>  Z   (?  �   �?  �   F@  M   A  0   jA  ]   �A  �   �A  C  �B  m   �C  �   hD  X   6E  V   �E  7  �E  �   G  �   �G  �   8H    �H  e   �I  �   QJ  }   �J  �   hK  �   BL  �   �L    �M  *  �N  �   �O  �   �P  �   NQ  u  $R     �S  z   T  �   �T  �  mU  *  W  �   FX  �   Y  ?   �Y  �   �Y  �   �Z  ~  <[  '   �\  �   �\  �   �]  �   &^  �   �^  �   �_  G  ?`  �   �a  �   sb  �   c  �   �c  q  `d     �e  �   �f  j   �g  �   h  �   �h  �   'i  �   $j  �   �j  X  �k                       /   =   "       #                    B          5                     *   ;            	   
       ?         7       9   (   6   0   A      &         +   L              :       $           F      -   J             .   <   8   )                1   ,      '              4              K   G   3   D   !   >   I       2      %      @           H   E   C    'Ignore' to continue waiting for the thread to respond.
'Cancel' to attempt to cancel the thread.
'Terminate' to quit PCSX2 immediately.
 (Folder type only) Re-index memory card content every time the running software changes. This prevents the memory card from running out of space for saves. (FolderMcd) Memory Card is full, could not add: %s All plugins must have valid selections for %s to run.  If you are unable to make a valid selection due to missing plugins or an incomplete install of %s, then press Cancel to close the Configuration panel. Avoids memory card corruption by forcing games to re-index card contents after loading from savestates.  May not be compatible with all games (Guitar Hero). Check HDLoader compatibility lists for known games that have issues with this (often marked as needing 'mode 1' or 'slow DVD'). Check this to force the mouse cursor invisible inside the GS window; useful if using the mouse as a primary control device for gaming.  By default the mouse auto-hides after 2 seconds of inactivity. Completely closes the often large and bulky GS window when pressing ESC or pausing the emulator. Convert this memory card to a 16 MB Memory Card .ps2 file. Convert this memory card to a 32 MB Memory Card .ps2 file. Convert this memory card to a 64 MB Memory Card .ps2 file. Convert this memory card to a standard 8 MB Memory Card .ps2 file. Default cyclerate. Runs the emulated PS2 Emotion Engine at normal speed. Disables EE Cycle Skipping. Most compatible setting. Enable this if you think MTGS thread sync is causing crashes or graphical errors. Enables automatic mode switch to fullscreen when starting or resuming emulation. You can still toggle fullscreen display at any time using alt-enter. Existing %s settings have been found in the configured settings folder.  Would you like to import these settings or overwrite them with %s default values?

(or press Cancel to select a different settings folder) Failed to read/write NVM/MEC file. Check your BIOS setup/permission settings. Failed: Destination memory card '%s' is in use. Failed: Duplicate is only allowed to an empty PS2-Port or to the file system. Gamefixes can work around wrong emulation in some titles. 
They may also cause compatibility or performance issues.

The safest way is to make sure that all game fixes are completely disabled. It's better to enable 'Automatic game fixes' at the main menu instead, and leave this page empty ('Automatic' means: selectively use specific tested fixes for specific games). Manual game fixes will NOT increase your performance. In fact they may decrease it. Known to affect following games:
 * Bleach Blade Battler
 * Growlanser II and III
 * Wizardry Known to affect following games:
 * Digital Devil Saga (Fixes FMV and crashes)
 * SSX (Fixes bad graphics and crashes)
 * Resident Evil: Dead Aim (Causes garbled textures) Known to affect following games:
 * Mana Khemia 1 (Going "off campus")
 Known to affect following games:
 * Test Drive Unlimited
 * Transformers Makes the emulated Emotion Engine skip cycles, allowing VU microprograms to execute at faster intervals. Helps a small subset of games with VU starvation problems, E.g. Shadow of the Colossus. More often than not this is harmful to performance and causes FPS readouts to be inaccurate. Maximum EE Cycle Skipping. Mostly harmful. May help games with significant VU starvation problems run at full speed. Mild EE Cycle Skipping. Mild slow down for most games, but may help some games with mild VU starvation problems run at full speed. Moderate EE Cycle Skipping. Slow down for most games, but may help some games with moderate VU starvation problems run at full speed. Modifies the emulated Emotion Engine CPU clock. Higher values may increase the internal framerate in games with variable framerates, but will increase CPU requirements substantially. Lower values will reduce the CPU load allowing lightweight games to run full speed on weaker CPUs. NTFS compression can be changed manually at any time by using file properties from Windows Explorer. NTFS compression is built-in, fast, and completely reliable; and typically compresses memory cards very well (this option is highly recommended). Note that when Framelimiting is disabled, Turbo and SlowMotion modes will not be available either. Note: Recompilers are not necessary for PCSX2 to run, however they typically improve emulation speed substantially. You may have to manually re-enable the recompilers listed above, if you resolve the errors. Notice: Due to PS2 hardware design, precise frame skipping is impossible. Enabling it will cause severe graphical errors in some games. Notice: Most games are fine with the default options. Off: Disables temporary aspect ratio switch.

4:3: Temporarily switch to a 4:3 aspect ratio while an FMV plays to correctly display an 4:3 FMV.

16:9: Temporarily switch to a 16:9 aspect ratio while an FMV plays to correctly display a widescreen 16:9 FMV. Out of Memory (sorta): The SuperVU recompiler was unable to reserve the specific memory ranges required, and will not be available for use.  This is not a critical error, since the sVU rec is obsolete, and you should use microVU instead anyway. :) XBSX2 is unable to allocate memory needed for the PS2 virtual machine. Close out some memory hogging background tasks and try again. PCSX2 requires a *legal* copy of the PS2 BIOS in order to run games.
You cannot use a copy obtained from a friend or the Internet.
You must dump the BIOS from your *own* Playstation 2 console. PCSX2 requires a PS2 BIOS in order to run.  For legal reasons, you *must* obtain a BIOS from an actual PS2 unit that you own (borrowing doesn't count).  Please consult the FAQs and Guides for further instructions. Please ensure that these folders are created and that your user account is granted write permissions to them -- or re-run PCSX2 with elevated (administrator) rights, which should grant PCSX2 the ability to create the necessary folders itself.  If you do not have elevated rights on this computer, then you will need to switch to User Documents mode (click button below). Please note that the resulting file may not actually contain all saves, depending on how many are in the source memory card. Please select a valid BIOS.  If you are unable to make a valid selection then press Cancel to close the Configuration panel. Please select your preferred default location for PCSX2 user-level documents below (includes memory cards, screenshots, settings, and savestates).  These folder locations can be overridden at any time using the Plugin/BIOS Selector panel. Primarily targetting the EE idle loop at address 0x81FC0 in the kernel, this hack attempts to detect loops whose bodies are guaranteed to result in the same machine state for every iteration until a scheduled event triggers emulation of another unit.  After a single iteration of such loops, we advance to the time of the next event or the end of the processor's timeslice, whichever comes first. Runs VU1 on its own thread (microVU1-only). Generally a speedup on CPUs with 3 or more cores. This is safe for most games, but a few games are incompatible and may hang. In the case of GS limited games, it may be a slowdown (especially on dual core CPUs). Speedhacks usually improve emulation speed, but can cause glitches, broken audio, and false FPS readings.  When having emulation problems, disable this panel first. The PS2-slot %d has been automatically disabled.  You can correct the problem
and re-enable it at any time using Config:Memory cards from the main menu. The specified path/directory does not exist.  Would you like to create it? There is not enough virtual memory available, or necessary virtual memory mappings have already been reserved by other processes, services, or DLLs. This action will reset the existing PS2 virtual machine state; all current progress will be lost.  Are you sure? This command clears %s settings and allows you to re-run the First-Time Wizard.  You will need to manually restart %s after this operation.

WARNING!!  Click OK to delete *ALL* settings for %s and force-close the app, losing any current emulation progress.  Are you absolutely sure?

(note: settings for plugins are unaffected) This folder is where PCSX2 loads cheats from. This folder is where PCSX2 records savestates; which are recorded either by using menus/toolbars, or by pressing F1/F3 (save/load). This folder is where PCSX2 saves its logfiles and diagnostic dumps.  Most plugins will also adhere to this folder, however some older plugins may ignore it. This folder is where PCSX2 saves screenshots.  Actual screenshot image format and style may vary depending on the GS plugin being used. This hack works best for games that use the INTC Status register to wait for vsyncs, which includes primarily non-3D RPG titles. Games that do not use this method of vsync will see little or no speedup from this hack. This is the folder where PCSX2 saves your settings, including settings generated by most plugins (some older plugins may not respect this value). This recompiler was unable to reserve contiguous memory required for internal caches.  This error can be caused by low virtual memory resources, such as a small or disabled swapfile, or by another program that is hogging a lot of memory. This wizard will help guide you through configuring plugins, memory cards, and BIOS.  It is recommended if this is your first time installing %s that you view the readme and configuration guide. Updates Status Flags only on blocks which will read them, instead of all the time. This is safe most of the time, and Super VU does something similar by default. Vsync eliminates screen tearing but typically has a big performance hit. It usually only applies to fullscreen mode, and may not work with all GS plugins. WARNING: Converting a memory card may take a while! Please do not close the emulator during the conversion process, even if the emulator is no longer responding to input. Warning!  Changing plugins requires a complete shutdown and reset of the PS2 virtual machine. PCSX2 will attempt to save and restore the state, but if the newly selected plugins are incompatible the recovery may fail, and current progress will be lost.

Are you sure you want to apply settings now? Warning!  You are running PCSX2 with command line options that override your configured plugin and/or folder settings.  These command line options will not be reflected in the settings dialog, and will be disabled when you apply settings changes here. Warning!  You are running PCSX2 with command line options that override your configured settings.  These command line options will not be reflected in the Settings dialog, and will be disabled if you apply any changes here. Warning: Some of the configured PS2 recompilers failed to initialize and have been disabled: When checked this folder will automatically reflect the default associated with PCSX2's current usermode setting.  You are about to delete the formatted memory card '%s'. All data on this card will be lost!  Are you absolutely and quite positively sure? You can change the preferred default location for PCSX2 user-level documents here (includes memory cards, screenshots, settings, and savestates).  This option only affects Standard Paths which are set to use the installation default value. You may optionally specify a location for your PCSX2 settings here.  If the location contains existing PCSX2 settings, you will be given the option to import or overwrite them. Your system is too low on virtual resources for PCSX2 to run. This can be caused by having a small or disabled swapfile, or by other programs that are hogging resources. Zoom = 100: Fit the entire image to the window without any cropping.
Above/Below 100: Zoom In/Out
0: Automatic-Zoom-In untill the black-bars are gone (Aspect ratio is kept, some of the image goes out of screen).
  NOTE: Some games draw their own black-bars, which will not be removed with '0'.

Keyboard: CTRL + NUMPAD-PLUS: Zoom-In, CTRL + NUMPAD-MINUS: Zoom-Out, CTRL + NUMPAD-*: Toggle 100/0 Project-Id-Version: PCSX2 - 190526
Report-Msgid-Bugs-To: https://github.com/PCSX2/pcsx2/issues
PO-Revision-Date: 2019-05-26 03:16+0200
Last-Translator: pgert <http://forums.pcsx2.net/User-pgert>
Language-Team: http://forums.pcsx2.net/Thread-Swedish-GUI-translation-for-PCSX2-Svenskt-anv%C3%A4ndargr%C3%A4nssnitt-f%C3%B6r-PCSX2
Language: sv_SE
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Poedit-KeywordsList: pxE;pxEt
X-Poedit-SourceCharset: UTF-8
X-Poedit-Basepath: trunk
X-Generator: Poedit 2.2.3
Plural-Forms: nplurals=2; plural=(n != 1);
X-Poedit-SearchPath-0: pcsx2
X-Poedit-SearchPath-1: common
 ''Förbise'' för att fortsätta vänta på trådarna att svara.
''Avbryt'' för att försöka avbryta tråden.
''Avsluta'' för att omedelebart stänga PCSX2.
 Återindexerar MinneskortsInnehåll var gång den aktiva mjukvaran ändras,
 vilket förhindrar att Minneskortet får slut på plats för sparningar (Endast mapp-typ). (MappMcd) Minneskortet är fullt - Följaktligen kunde inte denna tilläggas: %s Alla InsticksProgram måste ha giltliga inställningar för att %s ska kunna köras.
Är Ni oförmögen att göra detta till följd av saknade InsticksProgram eller en ofullständig
 installaion av %s, så tryck ''Avbryt'' för att stänga KonfigurationsDialogen. Undviker förstörelse av Minneskort genom att framtvinga spel att återindexera kortinnehåll
 efter en laddning av en Sparpunkt. Måhända inte förenligt med alla spel (''Guitar Hero''). Undersök HDLoader's förenlighetslista för spel
 som till vetskap kommer till fråga med det här
 (ofta markerat som behövande ''läge 1'' eller ''långsam DVD''). Markera för att få Muspekaren att döljas då den är i ett GS-fönster;
 användbart vid brukande av musen som främsta styrmojäng för spelande.
Som praxis gömmer sig Muspekaren automatiskt efter 2 sekunders inaktivitet. Stänger helt det ofta stora och omfångsrika GS-fönstret
 då Ni trycker på ''Esc'' och/eller pausar emulatorn. Omvandla detta Minneskort till en 16 MB Minneskorts .ps2 fil. Omvandla detta Minneskort till en 32 MB Minneskorts .ps2 fil. Omvandla detta Minneskort till en 64 MB Minneskorts .ps2 fil. Omvandla detta Minneskort till en vanlig 8 MB Minneskorts .ps2 fil. FörvalsCykelgrad. Kör den emulerade
 PS2'ns EE med normal hastighet. Förhindrar EE-Cykelöverhoppning.
Det mest förenliga läget. Tillämpa detta ifall Ni tror att MTGS-trådsynken
 vållar brakningar eller grafiska fel. Möjliggör automatiskt lägesbyte till Helskärm då emulering påbörjas eller återupptas.
Ni kan fortfarande växla mellan Helskärm och Inramningsläge genom att trycka ''Alt'' + ''Enter''. Befintliga %s-inställningar har hittats
 i KonfigurationsInställningsMappen.
Vill Ni importera dessa inställningar
 och överskriva %s's förval med dem?

 (eller tryck ''Avbryt'' för att välja en annan mapp) Misslyckades att läsa/skriva NVM/MEC fil. Undersök Era BIOS inställningar. Misslyckades: mål-Minneskortet '%s' är i bruk. Misslyckades: Dubblering är endast tillåtet till en tom PS2-sockel, eller till filsystemet. Spelfixar kan åtgärda felaktig emulering för vissa titlar.
De kan dock vålla förenlighets- och/eller prestandaproblem.

Genom att ha denna ruta Avmarkerad spärras samtliga Spelfixar. Det är över lag bättre att ha denna tillämpning spärrad, och att istället ha ''Automatiska Spelfixar'' möjliggjort
 i Huvudmenyn (''Automatiska'' medför en tillämpning av särskilt utvalda och beprövade lösningar för enskilda spel).
Egna Spelfixar ökar INTE prestandan, utan kan måhända istället minska den. Påverkar till vetskap följande spel:
 * ''Bleach Blade Battler''
 * ''Growlanser II & III''
 * ''Wizardry'' Påverkar till vetskap följande spel:
 * ''Digital Devil Saga'' (fixar FMV'er och brakningar)
 * ''SSX'' (fixar dålig grafik och brakningar)
 * ''Resident Evil: Dead Aim'' (vållar förvrängda texturer) Påverkar till vetskap följande spel:
 * ''Mana Khemia 1'' (går "bortom behörighet")
 Påverkar till vetskap följande spel:
 * ''Test Drive Unlimited''
 * ''Transformers'' Gör så att den emulerade EE'n hoppar över cykler,
 vilket möjliggör VU-microprogram att köras med snabbare intervall.
Hjälper en mindre mängd av spel med VU-svältproblem,
 såsom ''Shadow of the Colossus''. 
Oftare än sällan är detta skadligt för prestanda,
 och vållar felaktiga FPS-avläsningar. Högsta möjliga EE-Cykelöverhoppning.
Mestadels skadlig. Hjälper måhända spel
 med betydliga VU-svältproblem att köras i full fart. Blid EE-Cykelöverhoppning.
Blid maskning för de flesta spel, men hjälper måhända
 spel med blida VU-svältproblem att köras i full fart. Måttlig EE-Cykelöverhoppning.
Maskning för de flesta spel, men hjälper måhända vissa
 spel med måttliga VU-svältproblem att köras i full fart. Ändrar den emulerade EE-CPU-klockan.
Högre värden ökar måhända den egna Cykelgraden för spel
 med ombytlig Bildfrekvens, men kommer öka CPU-kraven väsentligt.
Lägre värden miskar CPU-kraven, vilket möjliggör
 lättviktiga spel att köras i full fart med klenare CPU'er. NTFS-komprimering kan egenhändigt
 ändras när som helst genom
 Egenskaper hos Windows Utforskaren. NTFS-komprimering är inbyggt, snabbt, och helt
 tillförlitligt; det komprimerar särligt Minneskort
 mycket bra (denna tillämpning anrådes tydligt). Bemärk att då Bildhastighetsreglering är
 spärrad så kommer lägena Turbo & Nedsatt
 heller inte att vara tillgängliga. Bemärk: Omkompilerare krävs inte för att PCSX2 ska kunna köras, men förbättrar särligt EmuleringsHastigheten avsevärt. Ni kanske måste återmöjliggöra omkompilerarna ovanför egenhändigt, om Ni löser fel. Bemärk: Till följd av utformningen av PS2's hårdvara
 så är precist bildöverhoppande ouppnåeligt.
Att tillämpa det kan vålla rejäla grafikfel för vissa spel. VM = Virtual Machine = Virtuell Maskin
VU = Vector Unit = Vektor Enhet
EE = Emotion Engine = Rörelse Motor = PS2 CPU'n
IOP = Input/Output Processor = In/Ut Processor

Bemärk: De flesta spel går bra med förvalssättningarna. Av: Förhindrar tillfällig BildkvotsVäxlare.

4:3: Växlar tillfälligt till 4:3 Bildkvot medans en FMV spelas
 för att riktigt återge en 4:3 FMV.

16:9: Växlar tillfälligt till 16:9 Bildkvot medans en FMV spelas
 för att riktigt återge en 16:9 FMV. Slut på Minne (typ): superVU omkompileraren var oförmögen att reservera
 den mängd särskilda minne som krävs, och kommer inte vara tillgänglig för bruk.
Detta är inte ett kritiskt fel eftersom sVU är föråldrat,
 och följaktligen borde Ni hur som helst använda Er av microVU istället. PCSX2 är oförmögen att tilldela det minne som krävs
 för PS2's Virtuella Maskin. Stäng minneskrävande
 bakgrundsprogram och försök igen. PCSX2 kräver *lagligt* PS2 BIOS för att köra spel.
Ni får inte använda en kopia anskaffat genom en vän eller Internet,
 utan måste hämta BIOS'et från Er *egna* PlayStation®2 konsol. PCSX2 kräver PS2 BIOS för att köras. Av juridiska skäl *måste* Ni anskaffa ett BIOS från en faktisk PS2 enhet som Ni äger (tillåns gäller inte). Öga igenom FAQ'er och Guider för ytterligare information. Försäkra Er om att dessa mappar är skapade och att Er AnvändarBehörighet medger skrivning för dem -- eller återkör PCSX2 med upphöjd (Administratörs) Behörighet, vilket borde bevilja PCSX2 att skapa de nödvändiga mapparna självt. Om Ni inte har upphöjd Behörighet för Er dator, får Ni byta till AnvändarnivåDokument-läge (klicka på knappen nedanför). Bemärk att slutfilen måhända inte innehåller samtliga sparningar, beroende på hur många som är med i käll-Minneskortet. Välj ett giltligt BIOS. Är Ni oförmögen att göra detta
 så tryck ''Avbryt'' för att stänga KonfigurationsDialogen. Välj Ert föredragna mål för PCSX2's AnvändarnivåDokument nedanför
 (innefattar Minneskort, Skärmbilder, Inställningar, & Sparpunkter).
Detta kan Ni ändra när som helst genom dialogen SystemInställningar. Främst inriktat på EE-tomgångsloop hos adress 0x81FC0 i kärnan,
 försöker denne fix att upptäcka loopar vars kroppar
 garanterat medför samma maskintillstånd vid varje upprepning
 tills en schemalagd händelse utlöser emulering av en annan enhet.
Efter en enskild upprepning av en sådan loop, fortskrider Vi till tiden
 för nästa händelse eller till slutet av processorns tidskvantum,
 vilket som än kommer först. Kör VU1 i en egen tråd (endast microVU1), vilket i allmänhet
 ger en uppsnabbning för CPU'er med 3 eller fler kärnor.
Detta är säkert för de flesta spel, men en del är oförenliga och kan strula.
Vidare kan en maskning ske för GS-begränsade spel
 (i synnerhet med dubbelkärniga CPU'er). UppsnabbningsTrimningar förbättrar oftast EmuleringsHastigheten, men kan vålla trassel,
 brutet ljud, och falska FPS-avläsningar. Spärra denna panel det första Ni gör vid EmuleringsProblem. PS2-sockeln %d har blivit automatiskt spärrad. Ni kan åtgärda problemet
 och återmöjliggöra sockeln när som helst genom ''Konfig > Minneskort'' i Huvudmenyn. Den angivna filvägen/mappen finns ej. Vill Ni låta skapa den? Det finns inte tillräckligt med Virtuellt Minne tillgängligt,
 eller så har den nödvändiga tilldelningen av Virtuellt Minne
 redan blivit reserverat av andra processer, tjänster eller DLL'er. Detta kommer att återställa det befintliga
 PS2 VM-tillståndet; alla nuvarande
 framsteg kommer att gå förlorade.
Är Ni säker? Detta rensar %s-inställningarna
 och låter Er att återköra KonfigurationsDialogen.
Ni får själv återstarta %s efter detta.

VARNING! Tryck ''Vidare'' för att rensa *SAMTLIGA* inställningar
 för %s och tvångstänga programet, med följd att
 alla nuvarande EmuleringsFramsteg går förlorade.
Bemärk: Inställningar för InsticksProgram påverkas ej.

Är Ni helt säker? Från denna mapp laddar PCSX2 fuskisar. I denna mapp sparar PCSX2 Sparpunkter, vilka antingen skapas genom användning
 av menyn/verktygsraden, eller genom att trycka på ''F1''/''F3'' (Skapa/Ladda). I denna mapp sparar PCSX2 sina loggfiler och diagnostisk-anteckningar.
De flesta InsticksProgram ansluter sig till denna mapp, men en del äldre kan förbise den. I denna mapp sparar PCSX2 Skärmbilder. Det faktiska SkärmbildsFormatet
 och stilen kan variera beroende på vilket GS-InsticksProgram som används. Denne fix fungerar bäst för spel som använder Statusregistret INTC
 för att invänta Vsync'ar, vilket främst omfattar icke-3D-RPG titlar.
Spel som inte använder denna Vsync-metod
 kommer på sin höjd att få en Liten uppsnabbning. Detta är mappen där PCSX2 sparar Era inställningar, samt inställningar
 skapade av de flesta InsticksProgram (dock kanske inte av vissa äldre). Denna omkompilerare var oförmögen att reservera det kringliggande
 minne som krävs för dess egna förråd, ett fel som kan ha orsakats av
 en låg tillgång av Virtuellt Minne, exempelvis till följd av att en (alltför) liten eller
 spärrad växlingsfil används, eller av att ett annat program tar för sig mycket minne. Denna Dialog kommer att hjälpleda Er genom konfigurering av InsticksProgram,
 Minneskort samt BIOS. Om detta är första gången Ni gör installation %s,
 anrådes Ni att flukta igenom ''Läs mig'' och ''KonfigurationsVägledningen''. Uppdaterar endast Statusflaggor för block
 som kommer att läsa dem, istället för alltid.
Detta är för det mesta säkert,
 och superVU gör något liknande som norm. Vsync slår ut skärmsönderrivning men har särligt en stor prestationseffekt.
Det tillämpas vanligtvis bara i Helskärmsläge,
 och kanske inte fungerar för alla GS-InsticksProgram. VARNING: Omvandling av Minneskort kan ta sin tid! Stäng inte av emulatorn
 under omvandlingsprocessen, även om den ej längre svarar. Varning! Att byta InsticksProgram kräver en full nedstängning
 och återställning av PS2's Virtuella Maskin.
PCSX2 kommer att försöka att spara och återställa tillståndet,
 men om de nyligen valda InsticksProgramen är oförenliga kan detta
 misslyckas, och alla nuvarande framsteg går således förlorade.

Är Ni säker att Ni vill tillämpa ändringarna nu? Varning! Ni kör PCSX2 med InstruktionsMöjligheter som åsidosätter Era Insticks- och/eller MappKonfigurations- Inställningar. Detta åskådliggörs inte i InställningsDialogen, och kommer att spärras om Ni tillämpar InställningsFörändringar här. Varning! Ni kör PCSX2 med InstruktionsMöjligheter som åsidosätter Era KonfigurationsInställningar. Detta åskådliggörs inte i InställningsDialogen, och kommer att spärras om Ni tillämpar några förändringar här. Varning: Några av de konfigurerade PS2 omkompilerarna
 kunde inte köras igång och har blivit spärrade: Då markerad kommer denna mapp automatiskt att fungera enligt
 förvalen förknippade med PCSX2's nuvarande AnvändarInställningar.  Ni är på väg att kassera det formaterade Minneskortet '%s' .
All data på detta kort kommer att förloras!
Är Ni helt säker? Ni kan ändra det föredragna förvalsmålet för PCSX2's AnvändarnivåDokument här
 (vilket innefattar Minneskort, Skärmbilder, Inställningar, & Sparpunkter).
Detta påverkar bara StandardFilvägarna, vilka ställs in efter InstallationsFörvalen. Ni kan förslagsvis ange en placering för Era PCSX2-inställningar här. Om placeringen innehåller befintliga PCSX2-inställningar kommer Ni ges möjlighet att importera eller överskriva. Ert system har för lite virtuella resurser för att PCSX2 ska kunna köras.
Detta kan vara till följd av att en (alltför) liten eller spärrad växlingsfil används,
 eller av att andra program tar för sig av tillgångarna. Zoom = 100.00: Anpassa hela bilden till fönstret utan beskärning;
 Över/Under 100.00: Zooma in/ut.
Bemärk: Zoom-värdet återställs inte automatiskt; det sparas i
 konfigurationsfilen, och tillämpas nästa gång Ni använder PCSX2.
Zoom = 0.00: Automatisk inzoomning tills de svarta räckena
 är borta (Bildkvot bibehålls, en del av bilden går utanför fönstret).
Bemärk: Vissa spel ritar sina egna svarta räcken
 vilka inte tas bort med ''0.00''.

Tangentbord:
 ''Ctrl'' + ''NumPad Plus'': Inzoomning
 ''Ctrl'' + ''NumPad Minus'': Utzoomning
 ''Ctrl'' + ''NumPad Stjärna'': Växla 100/0 