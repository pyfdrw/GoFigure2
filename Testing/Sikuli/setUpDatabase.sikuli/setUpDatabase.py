import os
address = os.getcwd()
#address = "home/es1076/GITROOT/GoFigure2/Testing/Sikuli/gofigure2_images"

#address = address+"/gofigure2_images"
print address
setBundlePath(address)

for create in 3:
	click("Settings.png")
	hover("Database_arrow.png")
	click("Set_up_database.png")
	assert exists("Create_user_for_MySQL_DB.png")
	if create == 1:
		click("cancel.png")
		assert not exists("Create_user_for_MySQL_DB.png")
	if create == 2:
		click("Next_arrow.png")
		assert exists("Please_fill_all_fields.png")
		click("ok.png")
	if create == 3:
		wait(1)
		type("gofigure" +Key.TAB + "gofigure")
		click("Next_arrow.png")
		assert exists("Root_Password.png")
	
		for root in 2
			if root == 1
				click("cancel.png")
				assert exists("There_is_problem_existing_users.png")
				assert not exists("Root_Password.png")
			if root == 2
				wait(1)
				type(Key.BACKSPACE)#+Key.BACKSPACE+Key.BACKSPACE+Key.BACKSPACE+Key.BACKSPACE+Key.BACKSPACE)
				click("ok.png")
				click("ok.png")

"1291065478425.png"wait(3)
type("First"+Key.TAB+Key.TAB+"Last"+Key.TAB+Key.ENTER)
click("ok.png")
click("Next_arrow.png")
wait(1)
type("microscope"+Key.TAB+Key.ENTER)
click("ok.png")
click("Finish.png")
click("ok.png")
