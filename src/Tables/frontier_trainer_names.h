#include "../../include/new/frontier.h"
/*
frontier_trainer_names.h
	set up tables of names for battle frontier trainers
	
arrays to edit:
	gMaleFrontierNamesTable
	gFemaleFrontierNamesTable
*/

extern const u8 sTrainerName_Liam[];
extern const u8 sTrainerName_Noah[];
extern const u8 sTrainerName_William[];
extern const u8 sTrainerName_James[];
extern const u8 sTrainerName_Logan[];
extern const u8 sTrainerName_Benjamin[];
extern const u8 sTrainerName_Mason[];
extern const u8 sTrainerName_Elijah[];
extern const u8 sTrainerName_Oliver[];
extern const u8 sTrainerName_Jacob[];
extern const u8 sTrainerName_Lucas[];
extern const u8 sTrainerName_Michael[];
extern const u8 sTrainerName_Alexander[];
extern const u8 sTrainerName_Ethan[];
extern const u8 sTrainerName_Daniel[];
extern const u8 sTrainerName_Matthew[];
extern const u8 sTrainerName_Aiden[];
extern const u8 sTrainerName_Henry[];
extern const u8 sTrainerName_Joseph[];
extern const u8 sTrainerName_Jackson[];
extern const u8 sTrainerName_Samuel[];
extern const u8 sTrainerName_Sebastian[];
extern const u8 sTrainerName_David[];
extern const u8 sTrainerName_Carter[];
extern const u8 sTrainerName_Wyatt[];
extern const u8 sTrainerName_Jayden[];
extern const u8 sTrainerName_John[];
extern const u8 sTrainerName_Owen[];
extern const u8 sTrainerName_Dylan[];
extern const u8 sTrainerName_Luke[];
extern const u8 sTrainerName_Gabriel[];
extern const u8 sTrainerName_Anthony[];
extern const u8 sTrainerName_Isaac[];
extern const u8 sTrainerName_Grayson[];
extern const u8 sTrainerName_Jack[];
extern const u8 sTrainerName_Julian[];
extern const u8 sTrainerName_Levi[];
extern const u8 sTrainerName_Christopher[];
extern const u8 sTrainerName_Joshua[];
extern const u8 sTrainerName_Andrew[];
extern const u8 sTrainerName_Lincoln[];
extern const u8 sTrainerName_Mateo[];
extern const u8 sTrainerName_Ryan[];
extern const u8 sTrainerName_Jaxon[];
extern const u8 sTrainerName_Nathan[];
extern const u8 sTrainerName_Aaron[];
extern const u8 sTrainerName_Isaiah[];
extern const u8 sTrainerName_Thomas[];
extern const u8 sTrainerName_Charles[];
extern const u8 sTrainerName_Caleb[];
extern const u8 sTrainerName_Josiah[];
extern const u8 sTrainerName_Christian[];
extern const u8 sTrainerName_Hunter[];
extern const u8 sTrainerName_Eli[];
extern const u8 sTrainerName_Jonathan[];
extern const u8 sTrainerName_Connor[];
extern const u8 sTrainerName_Landon[];
extern const u8 sTrainerName_Adrian[];
extern const u8 sTrainerName_Asher[];
extern const u8 sTrainerName_Cameron[];
extern const u8 sTrainerName_Leo[];
extern const u8 sTrainerName_Theodore[];
extern const u8 sTrainerName_Jeremiah[];
extern const u8 sTrainerName_Hudson[];
extern const u8 sTrainerName_Robert[];
extern const u8 sTrainerName_Easton[];
extern const u8 sTrainerName_Nolan[];
extern const u8 sTrainerName_Nicholas[];
extern const u8 sTrainerName_Ezra[];
extern const u8 sTrainerName_Colton[];
extern const u8 sTrainerName_Angel[];
extern const u8 sTrainerName_Brayden[];
extern const u8 sTrainerName_Jordan[];
extern const u8 sTrainerName_Dominic[];
extern const u8 sTrainerName_Austin[];
extern const u8 sTrainerName_Ian[];
extern const u8 sTrainerName_Adam[];
extern const u8 sTrainerName_Elias[];
extern const u8 sTrainerName_Jaxson[];
extern const u8 sTrainerName_Greyson[];
extern const u8 sTrainerName_Jose[];
extern const u8 sTrainerName_Ezekiel[];
extern const u8 sTrainerName_Carson[];
extern const u8 sTrainerName_Evan[];
extern const u8 sTrainerName_Maverick[];
extern const u8 sTrainerName_Bryson[];
extern const u8 sTrainerName_Jace[];
extern const u8 sTrainerName_Cooper[];
extern const u8 sTrainerName_Xavier[];
extern const u8 sTrainerName_Parker[];
extern const u8 sTrainerName_Roman[];
extern const u8 sTrainerName_Jason[];
extern const u8 sTrainerName_Santiago[];
extern const u8 sTrainerName_Chase[];
extern const u8 sTrainerName_Sawyer[];
extern const u8 sTrainerName_Gavin[];
extern const u8 sTrainerName_Leonardo[];
extern const u8 sTrainerName_Kayden[];
extern const u8 sTrainerName_Ayden[];
extern const u8 sTrainerName_Jameson[];

extern const u8 sTrainerName_Emma[];
extern const u8 sTrainerName_Olivia[];
extern const u8 sTrainerName_Ava[];
extern const u8 sTrainerName_Isabella[];
extern const u8 sTrainerName_Sophia[];
extern const u8 sTrainerName_Mia[];
extern const u8 sTrainerName_Charlotte[];
extern const u8 sTrainerName_Amelia[];
extern const u8 sTrainerName_Evelyn[];
extern const u8 sTrainerName_Abigail[];
extern const u8 sTrainerName_Harper[];
extern const u8 sTrainerName_Emily[];
extern const u8 sTrainerName_Elizabeth[];
extern const u8 sTrainerName_Avery[];
extern const u8 sTrainerName_Sofia[];
extern const u8 sTrainerName_Ella[];
extern const u8 sTrainerName_Madison[];
extern const u8 sTrainerName_Scarlett[];
extern const u8 sTrainerName_Victoria[];
extern const u8 sTrainerName_Aria[];
extern const u8 sTrainerName_Grace[];
extern const u8 sTrainerName_Chloe[];
extern const u8 sTrainerName_Camila[];
extern const u8 sTrainerName_Penelope[];
extern const u8 sTrainerName_Riley[];
extern const u8 sTrainerName_Layla[];
extern const u8 sTrainerName_Lillian[];
extern const u8 sTrainerName_Nora[];
extern const u8 sTrainerName_Zoey[];
extern const u8 sTrainerName_Mila[];
extern const u8 sTrainerName_Aubrey[];
extern const u8 sTrainerName_Hannah[];
extern const u8 sTrainerName_Lily[];
extern const u8 sTrainerName_Addison[];
extern const u8 sTrainerName_Eleanor[];
extern const u8 sTrainerName_Natalie[];
extern const u8 sTrainerName_Luna[];
extern const u8 sTrainerName_Savannah[];
extern const u8 sTrainerName_Brooklyn[];
extern const u8 sTrainerName_Leah[];
extern const u8 sTrainerName_Zoe[];
extern const u8 sTrainerName_Stella[];
extern const u8 sTrainerName_Hazel[];
extern const u8 sTrainerName_Ellie[];
extern const u8 sTrainerName_Paisley[];
extern const u8 sTrainerName_Audrey[];
extern const u8 sTrainerName_Skylar[];
extern const u8 sTrainerName_Violet[];
extern const u8 sTrainerName_Claire[];
extern const u8 sTrainerName_Bella[];
extern const u8 sTrainerName_Aurora[];
extern const u8 sTrainerName_Lucy[];
extern const u8 sTrainerName_Anna[];
extern const u8 sTrainerName_Samantha[];
extern const u8 sTrainerName_Caroline[];
extern const u8 sTrainerName_Genesis[];
extern const u8 sTrainerName_Aaliyah[];
extern const u8 sTrainerName_Kennedy[];
extern const u8 sTrainerName_Kinsley[];
extern const u8 sTrainerName_Allison[];
extern const u8 sTrainerName_Maya[];
extern const u8 sTrainerName_Sarah[];
extern const u8 sTrainerName_Madelyn[];
extern const u8 sTrainerName_Adeline[];
extern const u8 sTrainerName_Alexa[];
extern const u8 sTrainerName_Ariana[];
extern const u8 sTrainerName_Elena[];
extern const u8 sTrainerName_Gabriella[];
extern const u8 sTrainerName_Naomi[];
extern const u8 sTrainerName_Alice[];
extern const u8 sTrainerName_Sadie[];
extern const u8 sTrainerName_Hailey[];
extern const u8 sTrainerName_Eva[];
extern const u8 sTrainerName_Emilia[];
extern const u8 sTrainerName_Autumn[];
extern const u8 sTrainerName_Quinn[];
extern const u8 sTrainerName_Nevaeh[];
extern const u8 sTrainerName_Piper[];
extern const u8 sTrainerName_Ruby[];
extern const u8 sTrainerName_Serenity[];
extern const u8 sTrainerName_Willow[];
extern const u8 sTrainerName_Everly[];
extern const u8 sTrainerName_Cora[];
extern const u8 sTrainerName_Kaylee[];
extern const u8 sTrainerName_Lydia[];
extern const u8 sTrainerName_Aubree[];
extern const u8 sTrainerName_Arianna[];
extern const u8 sTrainerName_Eliana[];
extern const u8 sTrainerName_Peyton[];
extern const u8 sTrainerName_Melanie[];
extern const u8 sTrainerName_Gianna[];
extern const u8 sTrainerName_Isabelle[];
extern const u8 sTrainerName_Julia[];
extern const u8 sTrainerName_Valentina[];
extern const u8 sTrainerName_Nova[];
extern const u8 sTrainerName_Clara[];
extern const u8 sTrainerName_Vivian[];
extern const u8 sTrainerName_Reagan[];
extern const u8 sTrainerName_Mackenzie[];
extern const u8 sTrainerName_Madeline[];

const u8* const gMaleFrontierNamesTable[] =
{
	sTrainerName_Liam,
	sTrainerName_Noah,
	sTrainerName_William,
	sTrainerName_James,
	sTrainerName_Logan,
	sTrainerName_Benjamin,
	sTrainerName_Mason,
	sTrainerName_Elijah,
	sTrainerName_Oliver,
	sTrainerName_Jacob,
	sTrainerName_Lucas,
	sTrainerName_Michael,
	sTrainerName_Alexander,
	sTrainerName_Ethan,
	sTrainerName_Daniel,
	sTrainerName_Matthew,
	sTrainerName_Aiden,
	sTrainerName_Henry,
	sTrainerName_Joseph,
	sTrainerName_Jackson,
	sTrainerName_Samuel,
	sTrainerName_Sebastian,
	sTrainerName_David,
	sTrainerName_Carter,
	sTrainerName_Wyatt,
	sTrainerName_Jayden,
	sTrainerName_John,
	sTrainerName_Owen,
	sTrainerName_Dylan,
	sTrainerName_Luke,
	sTrainerName_Gabriel,
	sTrainerName_Anthony,
	sTrainerName_Isaac,
	sTrainerName_Grayson,
	sTrainerName_Jack,
	sTrainerName_Julian,
	sTrainerName_Levi,
	sTrainerName_Christopher,
	sTrainerName_Joshua,
	sTrainerName_Andrew,
	sTrainerName_Lincoln,
	sTrainerName_Mateo,
	sTrainerName_Ryan,
	sTrainerName_Jaxon,
	sTrainerName_Nathan,
	sTrainerName_Aaron,
	sTrainerName_Isaiah,
	sTrainerName_Thomas,
	sTrainerName_Charles,
	sTrainerName_Caleb,
	sTrainerName_Josiah,
	sTrainerName_Christian,
	sTrainerName_Hunter,
	sTrainerName_Eli,
	sTrainerName_Jonathan,
	sTrainerName_Connor,
	sTrainerName_Landon,
	sTrainerName_Adrian,
	sTrainerName_Asher,
	sTrainerName_Cameron,
	sTrainerName_Leo,
	sTrainerName_Theodore,
	sTrainerName_Jeremiah,
	sTrainerName_Hudson,
	sTrainerName_Robert,
	sTrainerName_Easton,
	sTrainerName_Nolan,
	sTrainerName_Nicholas,
	sTrainerName_Ezra,
	sTrainerName_Colton,
	sTrainerName_Angel,
	sTrainerName_Brayden,
	sTrainerName_Jordan,
	sTrainerName_Dominic,
	sTrainerName_Austin,
	sTrainerName_Ian,
	sTrainerName_Adam,
	sTrainerName_Elias,
	sTrainerName_Jaxson,
	sTrainerName_Greyson,
	sTrainerName_Jose,
	sTrainerName_Ezekiel,
	sTrainerName_Carson,
	sTrainerName_Evan,
	sTrainerName_Maverick,
	sTrainerName_Bryson,
	sTrainerName_Jace,
	sTrainerName_Cooper,
	sTrainerName_Xavier,
	sTrainerName_Parker,
	sTrainerName_Roman,
	sTrainerName_Jason,
	sTrainerName_Santiago,
	sTrainerName_Chase,
	sTrainerName_Sawyer,
	sTrainerName_Gavin,
	sTrainerName_Leonardo,
	sTrainerName_Kayden,
	sTrainerName_Ayden,
	sTrainerName_Jameson,
};

const u8* const gFemaleFrontierNamesTable[] =
{
	sTrainerName_Emma,
	sTrainerName_Olivia,
	sTrainerName_Ava,
	sTrainerName_Isabella,
	sTrainerName_Sophia,
	sTrainerName_Mia,
	sTrainerName_Charlotte,
	sTrainerName_Amelia,
	sTrainerName_Evelyn,
	sTrainerName_Abigail,
	sTrainerName_Harper,
	sTrainerName_Emily,
	sTrainerName_Elizabeth,
	sTrainerName_Avery,
	sTrainerName_Sofia,
	sTrainerName_Ella,
	sTrainerName_Madison,
	sTrainerName_Scarlett,
	sTrainerName_Victoria,
	sTrainerName_Aria,
	sTrainerName_Grace,
	sTrainerName_Chloe,
	sTrainerName_Camila,
	sTrainerName_Penelope,
	sTrainerName_Riley,
	sTrainerName_Layla,
	sTrainerName_Lillian,
	sTrainerName_Nora,
	sTrainerName_Zoey,
	sTrainerName_Mila,
	sTrainerName_Aubrey,
	sTrainerName_Hannah,
	sTrainerName_Lily,
	sTrainerName_Addison,
	sTrainerName_Eleanor,
	sTrainerName_Natalie,
	sTrainerName_Luna,
	sTrainerName_Savannah,
	sTrainerName_Brooklyn,
	sTrainerName_Leah,
	sTrainerName_Zoe,
	sTrainerName_Stella,
	sTrainerName_Hazel,
	sTrainerName_Ellie,
	sTrainerName_Paisley,
	sTrainerName_Audrey,
	sTrainerName_Skylar,
	sTrainerName_Violet,
	sTrainerName_Claire,
	sTrainerName_Bella,
	sTrainerName_Aurora,
	sTrainerName_Lucy,
	sTrainerName_Anna,
	sTrainerName_Samantha,
	sTrainerName_Caroline,
	sTrainerName_Genesis,
	sTrainerName_Aaliyah,
	sTrainerName_Kennedy,
	sTrainerName_Kinsley,
	sTrainerName_Allison,
	sTrainerName_Maya,
	sTrainerName_Sarah,
	sTrainerName_Madelyn,
	sTrainerName_Adeline,
	sTrainerName_Alexa,
	sTrainerName_Ariana,
	sTrainerName_Elena,
	sTrainerName_Gabriella,
	sTrainerName_Naomi,
	sTrainerName_Alice,
	sTrainerName_Sadie,
	sTrainerName_Hailey,
	sTrainerName_Eva,
	sTrainerName_Emilia,
	sTrainerName_Autumn,
	sTrainerName_Quinn,
	sTrainerName_Nevaeh,
	sTrainerName_Piper,
	sTrainerName_Ruby,
	sTrainerName_Serenity,
	sTrainerName_Willow,
	sTrainerName_Everly,
	sTrainerName_Cora,
	sTrainerName_Kaylee,
	sTrainerName_Lydia,
	sTrainerName_Aubree,
	sTrainerName_Arianna,
	sTrainerName_Eliana,
	sTrainerName_Peyton,
	sTrainerName_Melanie,
	sTrainerName_Gianna,
	sTrainerName_Isabelle,
	sTrainerName_Julia,
	sTrainerName_Valentina,
	sTrainerName_Nova,
	sTrainerName_Clara,
	sTrainerName_Vivian,
	sTrainerName_Reagan,
	sTrainerName_Mackenzie,
	sTrainerName_Madeline,
};

const u16 gNumMaleFrontierTrainerNames = ARRAY_COUNT(gMaleFrontierNamesTable);
const u16 gNumFemaleFrontierTrainerNames = ARRAY_COUNT(gFemaleFrontierNamesTable);
