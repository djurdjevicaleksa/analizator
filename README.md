# Zadatak A: Analizator prenosnog toka
Da bi se obavili svi scenariji koje smo imali u okviru prijema i reprodukcije satelitskog signala, neophodno je razumeti format prenosnog toka. Nakon što se ulazni stepen pozicionira na zadatu frekvenciju, i nakon što se signal deskrembluje, demultiplekser na svom ulazu raspolaže bitskim tokom u formatu MPEG-TS. U okviru implementacije u ovom zadatku, potrebno je raščlaniti ovaj format na njegove sastavne delove i prikazati njegov sadržaj u komandnoj liniji za potrebe analize. Umesto rada sa ulaznim stepenom, vaš softver treba da preuzme i parsira TS datoteku.

# Realizovani program treba da sadrži spregu za pokretanje iz komandne linije (Command Line Interface). Korisnik treba da ima mogućnost da odredi koju datoteku sa transportnim tokom želi da analizira preko ulaznih parametara programa.

## Minimalna funkcionalnost koju TS analizator treba da obezbedi:
- Detekcija veličine TS paketa
- Identifikacija svih pidova koji se pojavljuju u transportnom toku
- Prikaz semantike za svaki od navedenih pidova (Značenje podataka koji se prenose na datom pidu)
- Semantički prikaz sadržaja PAT, PMT, NIT i SDT tabela
- Prikaz informacije koji se kodek koristi u slučaju video i audio podataka
- Mapiranje pidova u grupe, na osnovu TV servisa kojem podaci na datom pidu pripadaju
- Prikaz pojedinačnih TS paketa u celosti u HEX formatu (svaki bajt je prikazan kao dve heksadecimalne cifre), uz dodatne semantičke podatke svih polja iz TS zaglavlja (TS header)

Za verifikaciju realizovanog rešenja koristiti alat DVB Inspector da biste uporedili vaš izlaz sa izlazom prikazanim u ovom alatu.