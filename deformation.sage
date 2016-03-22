#    deformation.sage: sage a wrapper for pancratz-tuitman code
#
#    Copyright (C) 2016, Edgar Costa
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 2 of the License, or
#    (at your option) version 3 of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.


import subprocess
import os


class deformation:
    """
    A class to manage the interface with deformation code
    """
    
    


    def __init__(self, f, p, d = 1, e = 0, t1 = 1, verbose = False):
        """
        t1 = t1 or t1 = gen^e
        """
        #FIXME
        #unless we know a smart way to pass q-adic numbers to flint, we are stuck with this 'hack'
        self.__verbose = verbose
        assert e == 0 or t1 == 1;
        if e == 0:
            self.__3rdargument = t1;
        else:
            self.__3rdargument = e;
                
        self.__f = f;
        self.__n = len(f.variables());
        self.__R = f.base_ring();
        # check the base ring == QQ(t)
        # FIXME?
        assert 1 == len(self.__R.base_ring().gens()), "coefficients are not in QQ(t)";
        assert QQ == self.__R.base_ring().base_ring(), "coefficients are not in QQ(t)";


        def t_poly_to_string(poly):
            if poly == 1:
                return ""
            
            out = str(poly.degree() + 1)+" ";
            for x in poly.list():
                out += " " + str(x);
            return "("+out+")"

        self.__f_input = "%s" % self.__n;
        for m, c in self.__f.dict().iteritems():
            self.__f_input +=" "+t_poly_to_string(c) + str(list(m)).replace(",","");


        self.__input = "%s\n%s\n%s\n" %(p, d, self.__3rdargument) + self.__f_input + "\n";
        
        self.__cp = None;

        # FIXME
        self.__binary = "/Users/edgarcosta/projects/pancratz-tuitman/examples/deformation_file_input";
        self.__env = dict(os.environ)
        self.__env['DYLD_LIBRARY_PATH'] = ".:/Users/edgarcosta/projects/pancratz-tuitman/"

    def frob_characteristic_polynomial(self):
        if self.__cp != None:
            return self.__cp[:];

        if self.__verbose:
            print "Computing the zeta function"
            print "it may take a while..."
        self.__inputfilename = tmp_filename();
        inputfile = open(self.__inputfilename, "w");
        inputfile.write(self.__input);
        inputfile.close();
        
        # FIXME env
        self.__process = subprocess.Popen([self.__binary, self.__inputfilename], stdout = subprocess.PIPE , stderr = subprocess.PIPE, env = self.__env)

        (self.__stdout, self.__stderr) = self.__process.communicate();

        if self.__process.returncode == 0:
            if self.__verbose:
                print "done!"
        else:
            print "Something went wrong!"
            print "Check self._deformation__stderr, self._deformation__stdout and self._deformation__process for more details.";
            return

        #process output
        poly_str = self.__stdout.split("\n")[-4].split(" ");
        poly = map(ZZ, poly_str[2:])
        self.__cp = PolynomialRing(ZZ,"T")(poly);
        len(poly) == int(poly_str[0]);
        return self.__cp;








        


        

