<%@ Language= "JScript" %>

<!-- #include file="loader.asp" -->

<%
//------------------------------------------------------------------------------
// this tool loads transaction lists from BofA into the main account
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// a little black raincloud, of course
//------------------------------------------------------------------------------
var database = new SQL_Database ();

var g_account_id = 1490;    // Checking (B of A) - Bretton
g_account_name = Get_Account_Name (database, g_account_id);

Handle_Form_Input (database);

var fso = new ActiveXObject ("Scripting.FileSystemObject");
var tx_file = fso.OpenTextFile (Server.MapPath ("Transactions") + "\\stmt_" + g_account_id + ".csv", 1);
var lines = new String ();
while ((! tx_file.AtEndOfStream) && g_should_continue)
{
    var line = tx_file.ReadLine ();
    if (line.match(/^\d\d/) != null)
    {
        // condition the line...
        var regexp = /\'/g;
        line = line.replace (regexp, "&#39");
            
        var entry_date = line.substr(0, 10);
        line = line.substr (11);
        var extract = line.match (/"?[^"]+"?,?/)[0];
        var entry_account = (line.match (/[^"]+/))[0];
        //Response.Write ("entry_account = " + entry_account + "<br>\n");

        line = line.substr (extract.length);
        var tx_amount = parseFloat ((line.split (",")[0]).match (/[^"]+/));

        if (! isNaN (tx_amount))
        {
            if (entry_account.match (/Beginning balance/i))
            {
                // skip this :)
            }
            else if (entry_account.match (/^Check\s\d+/i))
            {
                // Check 5197  
                var tx_method_id = Get_Method_Id (database, "Check");
                var tx_number = parseInt (entry_account.match (/\d+$/)[0], 10);
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                lines = Build_Line (database, null, tx_date, tx_method_id, tx_number, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/DES:PURCHASE/))
            {
                // STERLING 45 DES:PURCHASE CK#6002 GLENMD INDN:1400310000006713645605 CO ID:9017 -500  
                var tx_method_id = Get_Method_Id (database, "Check");
                var tx_number = parseInt (entry_account.substr (entry_account.search (/CK\#/) + 3).match (/^\d+/)[0], 10);
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sDES:PURCHASE/i));
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/PURCHASE/))
            {
                // WEGMANS 122 SH 11/26 #000461892 PURCHASE WEGMANS 122 SHAWA HUNT VALLEY MD
                // GRAUL'S MARKET 11/29 #000585724 PURCHASE GRAUL'S MARKET PARKTON MD","-42.92","2909.85"
                // GIANT FOOD INC 04/21 #000393862 PURCHASE GIANT FOOD INC #3 PASADENA MD","-41.83","10540.33"
                // Dominicks Stor 05/26 #000180885 PURCHASE Dominicks Store 1 PARK RIDGE IL","-202.93","19332.46"
                // HAN AH REUM MA 06/12 #000776167 PURCHASE HAN AH REUM MARKE CATONSVILLE MD","-44.46","10730.85"
                // GIANT 6271 09/11 #000647456 PURCHASE GIANT 6271 SHREWSBURY PA
                // SAFEWAY 1481 02/22 #000056525 PURCHASE 2610 BOSTON ST BALTIMORE MD
                // HARRIS TEETER 03/13 #000226218 PURCHASE 1801 Whetstone Wa Baltimore MD
                var tx_method_id = Get_Method_Id (database, "Debit Card");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.substr (entry_account.search (/\d\d\/\d\d\s\#/), 5)); 
                var tx_number = parseInt (entry_account.substr (entry_account.search (/ \#/) + 2).match (/^\d+/)[0], 10);
                var tx_account_nickname_a = entry_account.substr (0, entry_account.search (/\d\d\/\d\d/));
                var tx_account_nickname_b = entry_account.substr (entry_account.search (/PURCHASE/) + 9);
                var tx_account_nickname = (tx_account_nickname_b.search (tx_account_nickname_a) >= 0) ? tx_account_nickname_b : tx_account_nickname_a;
                //Response.Write ("A = " + tx_account_nickname_a + ", B = " + tx_account_nickname_b + ", XXX = " + tx_account_nickname + "<br>\n");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/External transfer fee/i))
            {
                // External transfer fee - 3 Day - 11/24/2010 Confirmation: 53737424
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/\d\d\/\d\d/)[0]); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Wire transfer fee/i))
            {
                // Wire Transfer Fee -25 
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/CHECKCARD/))
            {
                // CHECKCARD 1126 NAIL TRIX HUNT VALLEY MD 24224430331103025887705 
                // CHECKCARD 0608 RAZORBACK RAW BAR TOWSON MD 24246511160200742001003","-42.36","11203.31"
                // CHECKCARD 0609 NEIGHBORCARE ST JOSEPH TOWSON MD 24326881160286825700801","-291.14","10912.17"
                // CHECKCARD 0612 GREEN LEAF RESTAURANT HUNT VALLEY MD 24761971164274678010012","-38.70","10692.15"
                // CHECKCARD 0610 MICHAELS #4801 LUTHERVILLE MD 24610431162004010655152","-11.95","10680.20"
                // CHECKCARD 0613 HUNT VALLEY BP HUNT VALLEY MD 24122541164740164710039","-45.83","10527.66"
                // CHECKCARD 0614 HUNT VALLEY BP HUNT VALLEY MD 24122541165740165840131","-41.63","10482.03"
                // CHECKCARD 0613 CHANNAS BUFFET LANHAM MD 24071051165987146293028","-7.90","10474.13"
                // CHECKCARD 0615 HUNT VALLEY BP HUNT VALLEY MD 24122541166740166970282","-44.06","9430.07"
                //Response.Write ("entry_account = " + entry_account + "<br>\n");
                var tx_method_id = Get_Method_Id (database, "Visa Card");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.substr (10, 4)); 
                var tx_account_nickname = entry_account.substr (15).match (/^.*\s\d\d\d\d\d/)[0];
                var tx_number = parseInt (entry_account.match (/\d\d\d\d\d\d\d\d$/)[0], 10);
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/WITHDRWL/))
            {
                // BKOFAMERICA ATM 11/27 #000006416 WITHDRWL HUNT VALLEY SHAW COCKEYSVILLE MD
                // HERSHEY PR 07/10 #000004001 WITHDRWL 100 HERSHEY PRK D HERSHEY PA -202  
                // SOVEREIGN 09/11 #000784360 WITHDRWL 14655 MT. AIRY RO SHREWSBURY PA FE","-2.00","520.34"
                // SOVEREIGN 09/11 #000784360 WITHDRWL 14655 MT. AIRY RO SHREWSBURY PA","-203.00","937.38"
                if (entry_account.match (/ FE$/))
                {
                    var tx_method_id = Get_Method_Id (database, "Bank");
                    var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/\d\d\/\d\d/)[0]); 
                    var tx_account_nickname = "Bank of America";
                    var tx_type_id = Get_Type_Id (database, "Fee");
                    lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                }
                else
                {
                    var tx_method_id = Get_Method_Id (database, "ATM");
                    var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/\d\d\/\d\d/)[0]); 
                    var tx_account_nickname = "Cash";
                    var tx_type_id = Get_Type_Id (database, "Cash");
                    lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
                }
            }
            else if (entry_account.match (/DES:DIRECT/))
            {
                // NEXT CENTURY COR DES:DIRECT DEP ID:420023128612EA4 INDN:WADE,BRETTON CO ID:9111  
                var tx_method_id = Get_Method_Id (database, "EFT");
                var tx_type_id = Get_Type_Id (database, "Paycheck");
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sDES:DIRECT/i));
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                //var tx_number = parseInt (entry_account.substr (52, 8), 10);
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/DEPOSIT/))
            {
                // BKOFAMERICA ATM 01/05 #000003616 DEPOSIT HANOVER PKWY GREENBELT MD 14195  
                var tx_method_id = Get_Method_Id (database, "ATM");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/\d\d\/\d\d/)[0]); 
                var tx_type_id = Get_Type_Id (database, "Cash");
                //var tx_number = parseInt (entry_account.substr (52, 8), 10);
                lines = Build_Line (database, null, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Online Banking transfer to/i))
            {
                // Online Banking transfer to CHK 3475 Conf# 6189666581; Dale, BRETTON
                // Online Banking transfer to CHK 3475 Confirmation# 6189666581; Dale, BRETTON
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (27, 8); // CHK 0123 (an account type and last 4 digits)
                var tx_type_id = Get_Type_Id (database, "Transfer");
                var tx_number;
                if (entry_account.match (/Confirmation#/))
                {
                    tx_number = parseInt (entry_account.substr (50, 8), 10);
                }
                else if (entry_account.match (/Conf#/))
                {
                    tx_number = parseInt (entry_account.substr (42, 8), 10);
                }
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Online Banking transfer from/i))
            {
                // Online Banking transfer from SAV 3435 Conf# 0408289705 5000  
                // Online Banking transfer from SAV 3435 Confirmation# 0408289705 5000  
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (29, 8); // CHK 0123 (an account type and last 4 digits)
                var tx_type_id = Get_Type_Id (database, "Transfer");
                var tx_number;
                if (entry_account.match (/Confirmation#/))
                {
                    tx_number = parseInt (entry_account.substr (52, 8), 10);
                }
                else if (entry_account.match (/Conf#/))
                {
                    tx_number = parseInt (entry_account.substr (44, 8), 10);
                }
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Online Banking payment/))
            {
                // Online Banking payment to CRD 0294 Confirmation# 6408282717 -500  
                // Online Banking payment to CRD 0294 Conf# 6408282717 -500  
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (26, 8); // CHK 0123 (an account type and last 4 digits)
                var tx_type_id = Get_Type_Id (database, "Revolving Credit");
                var tx_number;
                if (entry_account.match (/Confirmation#/))
                {
                    tx_number = parseInt (entry_account.substr (49, 8), 10);
                }
                else if (entry_account.match (/Conf#/))
                {
                    tx_number = parseInt (entry_account.substr (41, 8), 10);
                }
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, tx_number, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/^TRANSFER/))
            {
                // TRANSFER BRETTON WADE:BRETTON WADE Confirmation# 4956053688 -4000  
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                lines = Build_Line (database, null, tx_date, tx_method_id, "0", tx_amount, 0) + lines;
            }
            else if (entry_account.match (/Interest Earned/i))
            {
                // Interest Earned 0.18  
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Interest");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Bill Payment/i))
            {
                // BGE Bill Payment -204.94  
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sBill Payment/i));
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/Counter Credit/) || entry_account.match (/Deposit/))
            {
                // Counter Credit  
                var tx_method_id = Get_Method_Id (database, "Teller");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                lines = Build_Line (database, null, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/Counter Debit/))
            {
                // 06/19/2012,"Counter Debit","-156.00","677.61"
                var tx_method_id = Get_Method_Id (database, "Teller");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                lines = Build_Line (database, null, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            /*
            else if (entry_account.match (/PAYPAL/))
            {
                // 06/06/201 DAVID REDDISH DES:IAT PAYPAL ID:4FYJ24YKYRK5G INDN:BRETTON WADE CO ID:7705 -500  
                var tx_method_id = Get_Method_Id (database, "Web");
            }
            */
            else if (entry_account.match (/WIRE TYPE/))
            {
                // WIRE TYPE:WIRE OUT DATE:120723 TIME:0507 ET TRN:2012072000281351 SERVICE REF:002507
                // WIRE TYPE:WIRE IN DATE: 110222 TIME:1635 ET TRN:2011022200411441 SEQ:110222160125H60 18445  
                var tx_method_id = Get_Method_Id (database, "EFT");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/DATE:\s*\d\d(\d\d\d\d)/)[1]); 
                lines = Build_Line (database, null, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/MD TLR cash withdrawal/))
            {
                // MD TLR cash withdrawal from CHK 5695 Banking Ctr HUNT VALLEY #50 -300  
                var tx_method_id = Get_Method_Id (database, "Teller");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Cash";
                var tx_type_id = Get_Type_Id (database, "Cash");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/FEE REVERSAL/))
            {
                // FEE REVERSAL INTERNATIONAL TRANSACTION FEE REVERSED 3.67  
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Refund");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            /*
            else if (entry_account.match (/DES:INST XFER/))
            {
                // PAYPAL DES:INST XFER ID:4FYJ24UDV6VHN INDN:BRETTON WADE CO ID:PAYP -200  
                var tx_method_id = Get_Method_Id (database, "Web");
            }
            */
            else if (entry_account.match (/Non-BKOFAMERICA ATM Transaction Fee/))
            {
                // Non-BKOFAMERICA ATM Transaction Fee -2  
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/NON-BANK OF AMERICA ATM FEE REFUND/))
            {
                // NON-BANK OF AMERICA ATM FEE REFUND FDES NMO 0006576 NBK2PKC 2  
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Refund");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/DES:FEE/))
            {
                // CHECK ORDER00358 DES:FEE ID:U022289112 INDN:BRETTON WADE 03-09 CO ID:0000 -26  
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/DES:WEB PYMT/))
            {
                // CARDMEMBER SERV DES:WEB PYMT ID:471924750245803 INDN:WADE,BRETTON S CO ID:5911 -305 
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sDES:WEB PYMT/i));
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/ATM\/CHECK CARD TEMPORARY CR/))
            {
                // ATM/CHECK CARD TEMPORARY CR ADJ ON 12/12/11 CARD # 4744770025836827 CLAIM #3252719D
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_account.match (/ADJ ON (\d\d\/\d\d)\/\d\d/)[1]); 
                var tx_account_nickname = "Nowhere";
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/INT CREDIT ADJ/))
            {
                // INT CREDIT ADJ INTEREST CREDIT ADJUSTMENT 3252719DEC11
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/DES:IAT PAYPAL/))
            {
                // AKITO TSUDA DES:IAT PAYPAL ID:4FYJ25J6RAMVL INDN:BRETTON WADE CO ID:7705 
                var tx_method_id = Get_Method_Id (database, "Web");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sDES:IAT PAYPAL/i));
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, 0) + lines;
            }
            else if (entry_account.match (/RETURN ITEM CHARGEBACK/))
            {
                // RETURN ITEM CHARGEBACK","-902.37","8243.94"
                // can I match the amount to determine where it came from? it was a deposit in this amount
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Nowhere";
                var tx_type_id = Get_Type_Id (database, "Returned Check");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/RETURNED ITEM CHARGEBACK FEE/))
            {
                // RETURNED ITEM CHARGEBACK FEE
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/OVERDRAFT ITEM FEE/))
            {
                // OVERDRAFT ITEM FEE FOR ACTIVITY OF 02-29 ELECTRONIC TRANSA
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/OVERDRAFT PROTECTION /))
            {
                // OVERDRAFT PROTECTION FROM 00446007423435 24.17 
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_type_id = Get_Type_Id (database, "Transfer");
                var tx_account_nickname = entry_account.substr (entry_account.search (/FROM\s/) + 5);
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/DES:PAYROLL/))
            {
                // Next Century Cor DES:PAYROLL ID:WADE INDN:Bretton S. Wade CO ID:1030 4313.31 
                var tx_method_id = Get_Method_Id (database, "EFT");
                var tx_type_id = Get_Type_Id (database, "Paycheck");
                var tx_account_nickname = entry_account.substr (0, entry_account.search (/\sDES:PAYROLL/i));
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                //var tx_number = parseInt (entry_account.substr (52, 8), 10);
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else if (entry_account.match (/Advantage Monthly Maintenance Fee/))
            {
                // Advantage Monthly Maintenance Fee -25 
                var tx_method_id = Get_Method_Id (database, "Bank");
                var tx_date = Compute_Transaction_Date (entry_date, entry_date); 
                var tx_account_nickname = "Bank of America";
                var tx_type_id = Get_Type_Id (database, "Fee");
                lines = Build_Line (database, tx_account_nickname, tx_date, tx_method_id, 0, tx_amount, tx_type_id) + lines;
            }
            else
            {
                Response.Write ("unmatched Tx<br>\n");
                lines += "<tr><td><table><tr><td>UNKNOWN</td><td>" + entry_date + "</td><td>" + entry_account + "</td><td>" + tx_amount + "</td></tr></table></td></tr>\n";
            }
        }
    } 
}
tx_file.Close ();

database.Close ();

//------------------------------------------------------------------------------
%>

<!-- #include file="loader_page.asp" -->
