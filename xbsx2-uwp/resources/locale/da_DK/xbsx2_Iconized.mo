��    L      |  e   �      p  �   q  �   �  2   �  �   �  �   �     5	  �   �	  `   |
  :   �
  :     :   S  B   �  H   �  4     Q   O  �   �  �   7  M     /   Y  M   �  �   �    �  ]   �  �   �  G   �  H   �    7  t   U  �   �  �   M    �  d   �  �   R  b   �  �   G  �     5   �  �   �  �   �  �   �  �   R  �     r  �  |   \  |   �  �   V  �  E   �   �!  �   �"  �   w#  J   $  �   [$  p   �$  G  a%  -   �&  �   �&  �   ['  �   �'  �   �(  �   Z)  �   �)  �   �*  �   �+  �   ?,  �   �,  *  �-  �   �.  �   �/  \   �0  r   �0  �   \1  �   �1  �   �2  �   �3  �  24    �5  �   �7  �   o8  =   9  �   S9  �   ?:  �   ;  �   �;  q   �<  8   �<  8   1=  8   j=  @   �=  M   �=  E   2>  ]   x>  �   �>  �   �?  G   X@  0   �@  R   �@  �   $A  
  �A  n   �B  �   aC  T   'D  U   |D  3  �D  �   F  �   �F  �   G  $  �G  Y   �H  �   (I  g   �I  �   .J  �   K  @   �K    �K  �   �L  �   �M  �   `N    O  m  !P  �   �Q  ~   .R  �   �R  �  �S  =  2U  �   pV  �   8W  6   �W  �   �W  �   �X  g  =Y  '   �Z  �   �Z  �   _[  �   \  �   �\  �   �]  �   ^  �   �^  �   �_  �   g`  �   1a  9  �a  �   %c  �   !d  e   e  �   me  �   �e  �   zf  �   sg  �   Ch  �  �h                       /   =   "       #                    B          5                     *   ;            	   
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

Keyboard: CTRL + NUMPAD-PLUS: Zoom-In, CTRL + NUMPAD-MINUS: Zoom-Out, CTRL + NUMPAD-*: Toggle 100/0 Project-Id-Version: PCSX2 1.5.0 - 190526
Report-Msgid-Bugs-To: https://github.com/PCSX2/pcsx2/issues
PO-Revision-Date: 2019-05-26 03:16+0200
Last-Translator: pgert <http://forums.pcsx2.net/User-pgert>
Language-Team: 
Language: da_DK
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
 'Ignorer' for fortsat at vente på at trådende svarer.
'Annullér' for at forsøge at afbryde tråden.
'Afbryd' for at afslutte PCSX2 med det samme.
 Genindekser indholdet på MindesKort hver gang den igangværende software ændrer sig.
Dette forhinder at MindesKortet løber tør for plads. (Virker kun til mapper) (FolderMcd) MindesKortet er fyldt op, kunne ikke tilføje: %s Der skal være valgt gyldige plugins for at %s kan køre. Hvis du ikke kan vælge et gyldigt plugin, fordi der mangler et plugin eller installationen af %s har slået fejl, skal du trykke på Annullér eller lukke Konfigurationsguiden. Undgår beskadigelse på MindesKort ved at tvinge spillet til at genindeksere kortindhold,
 efter det er blevet indlæst fra de gemte spiltilstande. Virker muligvis ikke til alle spil (Guitar Hero). Undersøg HDLoaders kompatibilitetsliste for spil,
 der er kendt for at have problemer med dette
 (ofte markeret som mangler ‘mode 1’ eller ‘slow DVD’). Kryds denne boks af for at tvinge musen til at forsvinde fra GS-vinduet;
 det er nyttigt, hvis man skal bruge musen til at spille.
Musen forsvinder automatisk fra skærmen
 efter to sekunder, hvis den ikke er blevet brugt. Lukker helt for det store og ressourcekrævende GS-vindue,
 når man trykker på ''Esc'' eller pauser emulatoren. Omdan dette MindesKort til en 16 MB MindesKort .ps2 fil. Omdan dette MindesKort til en 32 MB MindesKort .ps2 fil. Omdan dette MindesKort til en 64 MB MindesKort .ps2 fil. Omdan dette MindesKort til en standard 8 MB MindesKort .ps2 fil. StandardCyclusfrekvens. Kører den emulerede
 PS2'ns EE med normal hastighed. Forhindrer EE-CyclusOverspringning.
Den mest forenelige opstillingen. Aktivér dette hvis du mener at MTGS-trådsynkroniseringen
 er skyld i nedbrud og grafikfejl. Gør så fuldskærmsvisning automatisk starter,
 når man kører eller genoptager en emulering.
Du kan altid selv skfite til fuldskærmsvisning ved at trykke på ''Alt+Enter''. Der er fundet eksisterende %s indstillinger i mappen med indstillinger. Vil du importere disse indstillinger eller overskrive dem med %s standard værdier?

(eller tryk Annullér for at vælge en anden mappe) Kunne ikke læse/skrive NMV/MEC fil. Undersøg dine BIOS indstillinger. Mislykket: MindesKortet '%s' er allerede i brug. Mislykket: Det er kun muligt at kopiere til en tom PS2-port eller til filsystemet. Spilrettelser kan i særlige tilfælde forværre emuleringen.
De kan også skabe kompatibilitets- eller ydelsesproblemer.

Det sikreste er at sørge for at alle spilrettelser er slået helt fra. Det fungerer bedre, hvis man aktiverer ''Automatiske spilrettelser'' i hovedmenuen i stedet for
 (''Automatisk'' betyder: selektivt brug af specifikke testede rettelser til specifikke spil).
Manuelle spilrettelser forbedre IKKE ydeevnen. Den forværrer den muligvis. Kendt for at påvirke følgende spil:
 * ''Bleach Blade Battler''
 * ''Growlanser'' II and III
 * ''Wizardry'' Kendt for at påvirke følgende spil:
 * ''Digital Devil Saga'' (Ordner FMV og bryder ned)
 * ''SSX'' (Ordner grafikfejl og bryder ned)
 * ''Resident Evil: Dead Aim'' (Danner forkludrede teksturer) Kendt for at påvirke følgende spil:
 * ''Mana Khemia 1'' (Ryger ud af spilkortet)
 Kendt for at påvirke følgende spil:
 * ''Test Drive Unlimited''
 * ''Transformers'' Gør så at den emulerede EE'n springer over cycluser,
 hvilket muliggjør VU-microprogram at køres med hurtigere intervall.
Hjælper en mindre mængde av spil med VU-sultproblem,
 såsom Shadow of the Colossus.
Oftere end sjældent er dette skadeligt for præstanda
 og volder fejlaktige FPS-aflæsninger. Højeste mulige EE-CyclusOverspringning.
Meestendels skadelig. Måske hjælper spil
 med betydelige VU-sultproblem at køres i fuld fart. Blid EE-CyclusOverspringning.
Blid nedgang for de fleste spil, men måske hjælper
 spil med blide VU-sultproblem at køres i fuld fart. Moderat EE-CyclusOverspringning.
Nedgang for de fleste spil, men måske hjælper visse
 spil med moderate VU-sultproblem at køres i fuld fart. Ændrer den emulerede EE-CPU-klokken.
Højere værten øger måske den egne Cyclusfrekvensen for spil
 med vægelsindet billedfrekvens,
 men kommer øge CPU-kravene væsentligt.
Lavere værten mindsker CPU-kravene, hvilket muliggjør
 letvigtige spil at køres i fuld fart med svagere CPU'er. NTFS-kompression kan altid ændres manuelt ved brug af filegenskaber i Windows Stifinder. NTFS-kompression er indbygget, hurtigt og fuldstændig troværdig; og komprimerer typisk MindesKort uden problemer (denne valgmulighed er stærkt anbefalet). Bemærk, at når Framelimiting er slået fra,
 vil Turbo og Slowmotion heller ikke være tilgængelige. Bemærk: Recompilere er ikke nødvendige for at PCSX2 virker, men de forbedrer typisk hastigheden på emuleringen markant. Du skal muligvis manuelt genaktivere de ovenstående recompilere, hvis du løser fejlene. Bemærk: Grundet PS2’s design af hardware,
 er præcis frameskipping umuligt. Hvis du aktiverer det,
 vil det forårsage voldsomme grafiske fejl i nogle spil. Bemærk: De fleste spil kan spilles med standardindstillingerne. Af: Forhindrer den tilfældige BilledformatVekslingen.

4:3: Veksler tilfældigt til 4:3 Billedformat mens en FMV spilles
 for at rigtigt gengive en 4:3 FMV.

16:9: Veksler tilfældigt til 16:9 Billedformat mens en FMV spilles
 for at rigtigt gengive en 16:9 FMV. Ikke mere plads: SuperVU recompileren kunne ikke reservere den nødvendige hukommelse og vil ikke kunne bruges. Dette er ikke en kritisk fejl, da sVU rec er forældet, og du skal bruge microVU i stedet for. :) PCSX2 er ikke i stand til at allokere den nødvendige minde for at den virtuelle PS2-maskine kan tages i brug. Luk nogle af dine andre programmer og prøv igen. PCSX2 kræver en lovlig kopi af en PS2 BIOS for at man kan spille.
Man kan ikke bruge en kopi fra en ven eller fra internettet.
Du skal bruge BIOS'en fra din *egen* PlayStation®2 konsol. PCSX2 kræver en PS2 BIOS-fil for at kunne køre. Af retsmæssige årsager *skal* sådan en BIOS-fil erhverves fra en fysisk PS2, som du selv ejer (det tæller ikke at låne den). Vær venlig at læse FAQ'en og de forskellige guides for yderligere information. Sikr dig at disse mapper findes, og at din bruger har skriverettigheder til disse mapper -- eller prøv at lukke PCSX2 og start programmet op igen med administratorrettigheder. Det vil give PCSX2 mulighed for selv at danne disse mapper. Hvis du ikke selv har rettigheder til dette, skal du skifte til “Brugerdokumenter-modus” ved at klikke på knappen nedenfor. Vær opmærksom på at den følgende fil muligvis ikke indeholder alle gemte spilfiler. Det afhænger af hvor mange filer der er i det originale MindesKortet. Vælg venligst en gyldig BIOS-fil. Hvis du ikke kan vælge en, skal du trykke på Annullér for at lukke konfigurationsguiden. Vælg venligst din foretrukne standardplacering til PCSX2-dokumenter nedenfor (det inkluderer MindesKort, skærmbilleder, indstillinger og gemte spiltilstande). Disse mappeplaceringer kan overskrives når som helst via Plugin/BIOS vælger-panelet. Retter sig primært mod EE-idleloop’et på adresse 0x81FC0
 i kerneprogrammet. Dette hack forsøger at opdage sløjfer,
 hvis samlinger er garanteret at resultere i den samme
 maskinstilstand for hver gentagelse der er, indtil en planlagt
 begivenhed udløser emuleringen af en anden enhed.
Efter første gentagelse af en sløjfe,
 fortsætter vi til næste begivenhed eller til næste tidsdel. Kører VU1 på sin egen procestråd (kun MicroVU1).
Giver generelt en forøgelse af hastigheden på processorer
 med tre eller flere kerner. Dette fungerer til de fleste spil,
 men nogle spil vil kunne gå i stå. Hvis spil er begrænset til GS,
 kan det gøre spillene langsommere (især på en to-kernet processor). Speedhacks forbedrer normalt emuleringshastigheden,
 men kan forårsage fejl, ødelagt lyd, og forkerte aflæsninger af billeder pr. sekund.
Hvis du har problemer skal du slå dette panel fra først. PS2-slottet %d er blevet afbrudt automatisk. Du kan løse problemet
og genaktivere det ved at vælge Rediger > MindesKort fra hovedmenuen. Den angivne sti/mappe findes ikke. Vil du oprette den? Der er ikke nok virtuel hukommelse ledig, ellers er de nødvendige virtuelle hukommelseskortlægninger allerede reserveret af andre processer, tjenester eller DLL-filer. Denne handling vil nulstille den nuværende virtuelle PS2-maskines tilstand; al nuværende fremskridt vil gå tabt. Er du sikker på, du vil fortsætte? Denne kommando sletter %s indstillinger og lader dig køre installationsguiden forfra. Du skal genstarte %s manuelt efter denne handling.

ADVARSEL!! Tryk på OK for at slette *ALLE* indstillinger for %s og tvinge programmet til at lukke. Du vil miste al nuværende emuleringsfremskridt. Er du helt sikker?
(bemærk: plugin-indstillinger forbliver upåvirket) Fra denne mappe indlæser PCSX2 snyder. I denne mappe gemmer PCSX2 spiltilstande; som bliver gemt enten via menuerne/værktøjsbjælken, eller ved at trykke ''F1''/''F3'' (gem/indlæs). I denne mappe gemmer PCSX2 sine logfiler og diagnostiske dumps. De fleste plugins vil også holde sig til denne mappe, men nogle ældre plugins kan ignorere den. I denne mappe gemmer PCSX2 skærmbilleder. Formatet og stilen på skærmbillederne kan variere afhængig af hvilket GS plugin, der bliver brugt. Denne hack virker bedst i spil, der bruger INTC statusregisteret
 til at vente på lodrette synkroniseringer, primært RPG-titler uden 3D.
Spil,der ikke bruger denne metode,
 vil kun opleve en lille eller slet ingen hastighedsforbedring. I denne mappe gemmer PCSX2 dine indstillinger, deriblandt indstillinger genereret af de fleste plugins (ældre plugins har muligvis ikke denne egenskab). Recompileren kunne ikke reservere sammenhængende minde, der skal bruges til den indre cache. Fejlen kan skyldes en lav virtuel minde, såsom en lille eller en afbrudt swap-fil, eller et andet program, der bruger en masse minde. Guiden vil hjælpe dig med at konfigurere plugins, MindesKort og BIOS. Det anbefales, at du læser Readme-filen og Konfigurationsguiden, hvis du installerer %s for første gang. Opdaterer kun statusflaget på datablokke der vil læse dem,
 i stedet for hele tiden. Dette er sikkert det meste af tiden,
 og SuperVU’en gør normalt noget lignende af sig selv. VSync undgår forvrængning af skærmbilledet,
 men forværrer dog typisk ydeevnen.
Sædvanligvis bliver VSync kun anvendt i fuldskærmsvisning,
 og det virker muligvis ikke sammen med alle GS plugins. ADVARSEL: Det vil tage noget tid at konvertere et MindesKort! Du må ikke lukke ned for emulatoren under processen, heller ikke selvom emulatoren ikke længere reagerer på nogen input. Advarsel! Det kræver en fuldstændig genstart og nulstilling af den virtuelle PS2-maskine. PCSX2 vil forsøge at gemme og gendanne tilstanden, men hvis det nyvalgte plugin er inkompatibelt, kan gendannelsen fejle og de nuværende fremskridt vil gå tabt.

Er du sikker på, du vil anvende disse indstillinger nu? Advarsel! Du kører PCSX2 med kommandolinje-muligheder, der overskriver dine konfigurerede plugin- og/eller mappeindstillinger. Disse kommandolinjeparametre vil ikke vise sig i indstillingsvinduet og vil ikke længere virke, hvis du ændrer noget her. Advarsel! Du kører PCSX2 med kommandolinje-muligheder, der overskriver dine konfigurerede indstillinger. Disse kommandolinjeparametre vil ikke vise sig i indstillingsvinduet og vil ikke længere virke, hvis du ændrer noget her. Advarsel: Nogle af de konfigurerede PS2 recompilere kunne ikke initialisere og er blevet deaktiveret: Når denne boks er krydset af, afspejler denne mappe automatisk den standard, der er forbundet med PCSX2s aktuelle brugerindstilling.  Du er ved at slette det formaterede MindesKortet '%s'. Al data på dette kort vil gå tabt! Er du helt sikker på, du vil fortsætte? Du kan ændre din foretrukne standardplacering til PCSX2-dokumenter nedenfor (det inkluderer MindesKort, skærmbilleder, indstillinger og gemte spiltilstande). Denne valgmulighed påvirker kun standardstier, som bruger installationens normalværdi. Her kan du selv vælge, hvor du vil gemme dine PCSX2-indstillinger. Hvis placeringen indeholder allerede eksisterende indstillinger, vil du blive spurgt, om du vil importere eller overskrive indstillingerne. Dit system har ikke nok virtuelle ressourcer ledige for at PCSX2 kan køre. Dette kan skyldes, at du har en lille eller en ubrugelig swap-fil, eller at et andet program bruger al pladsen. Zoom = 100: Tilpas hele billedet til vinduet
 uden der bliver skåret noget væk.
Over/Under 100: Zoom ind/ud
0: Zoomer automatisk ind, så der ikke længere er nogen sorte kanter.
 (Aspektforholdet beholdes, men noget af billedet
   vil være uden for skærmområdet).
BEMÆRK: Nogle spil har deres egne sorte kanter,
 som ikke kan fjernes med '0'.
Tastaturgenveje:
 CTRL+NUMPAD-PLUS: Zoom-ind,
 CTRL+NUMPAD-MINUS: Zoom-ud,
 CTRL+NUMPAD-*: Skift mellem 100/0 