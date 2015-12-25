<?php
    
        $to = "immortals@ageoflegends.com"; 
        $from = "Majere <majere@ageoflegends.com>"; 
        $subject = "Latest Version of the Prayers Logs"; 
    
        $fileatt = "/home/aol/classic/log/prayer.full";
        $fileatttype = "text/plain"; 
        $fileattname = "prayers.txt";
    
        $headers = "From: $from";
        
?>

<?php
    
        $file = fopen( $fileatt, 'rb' ); 
        $data = fread( $file, filesize( $fileatt ) ); 
        fclose( $file );
        
?>

<?php
    
        $semi_rand = md5( time() ); 
        $mime_boundary = "==Multipart_Boundary_x{$semi_rand}x"; 
    
        $headers .= "\nMIME-Version: 1.0\n" . 
                    "Content-Type: multipart/mixed;\n" . 
                    " boundary=\"{$mime_boundary}\"";
    
        $message = "This is a multi-part message in MIME format.\n\n" . 
                "--{$mime_boundary}\n" . 
                "Content-Type: text/plain; charset=\"iso-8859-1\"\n" . 
                "Content-Transfer-Encoding: 7bit\n\n" . 
                "\n\n";
    
        $data = chunk_split( base64_encode( $data ) );
                 
        $message .= "--{$mime_boundary}\n" . 
                 "Content-Type: {$fileatttype};\n" . 
                 " name=\"{$fileattname}\"\n" . 
                 "Content-Disposition: attachment;\n" . 
                 " filename=\"{$fileattname}\"\n" . 
                 "Content-Transfer-Encoding: base64\n\n" . 
                 $data . "\n\n" . 
                 "--{$mime_boundary}--\n"; 
                 
?>

<?php
        
        if( mail( $to, $subject, $message, $headers ) ) {
         
            echo "<p>The email was sent.</p>"; 
         
        }
        else { 
        
            echo "<p>There was an error sending the mail.</p>"; 
         
        }
    
?>
