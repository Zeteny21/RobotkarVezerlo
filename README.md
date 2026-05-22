# PROJEKT DOKUMENTÁCIÓ

**Tárgy:** Digitális Technika  
**Projekt címe:** Ipari robotkar vezérlőrendszer
**Készítette:** Gajdos Dávid [JIEF0F] 

---

## 1. Bevezetés és a projekt célja
A modern ipar és az *Ipar 4.0 (Industry 4.0)* koncepciójának egyik legfontosabb alapja a folyamatok automatizálása és a robotika. A beadandó feladat keretein belül egy olyan mikrokontroller-alapú (Arduino) rendszert terveztem és szimuláltam, amely egy három szabadságfokú (3-DOF) ipari robotkar vezérlésének logikáját mutatja be.

A projekt nem csupán egy egyszerű, manuálisan irányítható rendszert valósít meg. A fő cél egy olyan rögzítés és visszajátszás funkció integrálása volt, amely lehetővé teszi, hogy a kezelő betanítson egy mozdulatsort a robotkarnak, amelyet az a későbbiekben emberi beavatkozás nélkül, végtelen ciklusban képes önállóan megismételni. A rendszer állapotáról egy 16x2-es LCD kijelző, valamint állapotjelző LED-ek adnak valós idejű visszajelzést a kezelőnek.

---

## 2. Gyakorlati alkalmazhatóság és ipari relevanciák
Ez a technológia – bár a projektben szervo motorokkal és potenciométerekkel van szimulálva – a valós életben számos kulcsfontosságú területen van jelen:

* **Teach Pendant (Betanító vezérlő) programozás:** Az ipari gyártósorokon (pl. KUKA, FANUC robotok) a mérnökök gyakran nem kódsorokkal programozzák a robotokat, hanem egy kézi vezérlővel a megfelelő pozíciókba mozgatják a kart, majd elmentik a pontokat. Ez a projekt pontosan ezt a logikát modellezi.
* **Cobotok (Kollaboratív robotok):** A modern robotikában egyre gyakoribb, hogy a gépkezelő fizikailag megfogja a robotkart, végigvezeti a kívánt útvonalon (pl. egy hegesztési varrat mentén), a robot pedig a beépített szenzoraival rögzíti a szögelfordulásokat, majd tökéletesen visszajátssza azokat.
* **Filmipar és kameramozgatás (Motion Control):** A filmforgatásokon vizuális effektek készítésénél használnak hasonló "Record & Playback" kameramozgató robotokat. A kameraman manuálisan felvesz egy mozgást, amit a robot utána hajszálpontosan megismétel.
* **Automatizált tesztelés:** Érintőképernyők, gombok vagy egyéb fizikai termékek élettartam-tesztelésénél a gép újra és újra ugyanazt a betanított mozdulatsort hajtja végre több ezerszer.

---

## 3. Szoftveres megvalósítás és programozási logika
A kódolás során a legfőbb kihívást az jelentette, hogy az Arduino nem végezhet olyan várakozási műveleteket (`delay()`), amelyek megakasztanák a program futását, miközben folyamatosan figyelni kell a gombokat és a potmétereket is.

### 3.1. Állapotgép architektúra
A program logikája egy állapotgép köré épül, amelynek egyetlen `currentMode` nevű változója határozza meg a rendszer viselkedését:
* **Mód 0 (Manuális):** A rendszer olvassa a potenciométerek értékeit a `map()` függvénnyel, és azonnal továbbítja a szervomotorok felé.
* **Mód 1 (Felvétel):** A manuális irányítás megmarad, de a rendszer adott időközönként elmenti a 3 szervó aktuális szögét egy memóriatömbbe.
* **Mód 2 (Lejátszás):** A rendszer ignorálja a potenciométereket. Ehelyett a memóriatömbökből olvassa ki a rögzített értékeket, és mozgatja a motorokat.

### 3.2. Aszinkron időzítés `millis()` függvénnyel
A hagyományos `delay()` függvény elkerülése érdekében a `millis()` függvényt alkalmaztam. Külön időzítők felelnek a motorok másodpercenként 10-szeres (100ms) pozíció-mentéséért/lejátszásáért, és egy másik, független időzítő felel az LCD kijelző másodpercenként 5-szörös (200ms) frissítéséért.

### 3.3. Memóriakezelés
A mozgási adatokat három egydimenziós tömb (`pos1`, `pos2`, `pos3`) tárolja. Az Arduino Uno korlátozott SRAM memóriája miatt a maximális lépésszámot (`MAX_STEPS`) 100-ban maximalizáltam, ami bőven elegendő egy alap mozdulatsor rögzítésére.

---

## 4. Rendszerhasználati útmutató
1. **Indítás:** Az áramkör bekapcsolása után az LCD kijelzőn egy inicializációs üzenet fogadja a kezelőt, majd a rendszer **Manuális módba** lép. A potenciométerek elforgatásával a szervomotorok mozognak.
2. **Rögzítés:** A felhasználó megnyomja a bal oldali gombot. A piros LED felgyullad, a kijelzőn a `Mod: Felvetel` felirat jelenik meg, és a számláló pörögni kezd. A felhasználó elvégzi a kívánt mozdulatsort a potméterekkel. Ha végzett, a gomb ismételt megnyomásával a felvétel leáll.
3. **Lejátszás:** A jobb oldali gomb lenyomásával a Zöld LED világít. A rendszer a memóriából elkezdi visszajátszani a lépéseket. A kijelzőn a `Mod: Lejatszas` felirat mellett a `Lepes: X/Y` visszajelzés mutatja a folyamatot. A lejátszás végtelenített ciklusban fut.

---

## 5. Összegzés és továbbfejlesztési lehetőségek
A projekt tökéletesen imitálja egy robotkar működésének alapjait.

**A jövőbeli továbbfejlesztési lehetőségek:**
* **EEPROM memória használata:** Jelenleg a rögzített mozgás a RAM-ban tárolódik, így áramszünet esetén elvész. Az Arduino belső EEPROM-jának használatával a betanított mozgás kikapcsolás után is megmaradna.
* **Inverz kinematika:** Ezzel a módszerrel a potenciométerek a robotkar "kézfejének" X-Y-Z térbeli koordinátáit állítanák be, amit az Arduino számolna át ízületi szögértékekké matematikai mátrixok segítségével.

---
