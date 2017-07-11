# x509hostmatch

Whether Certificate names (cn, subjectAlternativeName as dNSNames) should match a given hostname.

## Bulding

```
make x509hostmatch
```


## Results
```
 [ OK ]	                example.com should     match                   example.com
 [ OK ]	            www.example.com should     match               www.example.com
 [ OK ]	                example.com should not match               www.example.com
 [ OK ]	            www.example.com should not match                   example.com
 [ OK ]	              *.example.com should     match                 a.example.com
 [ OK ]	              *.example.com should     match                 b.example.com
 [ OK ]	              *.example.com should     match               www.example.com
 [ OK ]	              *.example.com should     match      xn--rger-koa.example.com
 [ OK ]	          *.www.example.com should     match          test.www.example.com
 [ OK ]	           *www.example.com should     match               www.example.com
 [ OK ]	 *.xn--rger-koa.example.com should     match  www.xn--rger-koa.example.com
 [ OK ]	              *.xn--bar.com should     match           xn--foo.xn--bar.com
 [ OK ]	        *.good--example.com should     match         www.good--example.com
 [ OK ]	             t*.example.com should     match              test.example.com
 [ OK ]	             *t.example.com should     match              test.example.com
 [ OK ]	              *.example.com should     match               www.example.com
 [ OK ]	              *.example.com should not match                   example.com
 [ OK ]	              *.example.com should not match           zzz.www.example.com
 [ OK ]	             f*.example.com should     match              frog.example.com
 [ OK ]	             f*.example.com should not match        frog.super.example.com
 [ OK ]	           baz*.example.net should     match              baz1.example.net
 [ OK ]	           *baz.example.net should     match            foobaz.example.net
 [ OK ]	            b*z.example.net should     match              buzz.example.net
 [ OK ]	         sub1.*.example.com should not match            sub1.a.example.com
 [ OK ]	            *.*.example.com should not match               a.a.example.com
 [ OK ]	                      *.com should not match                         a.com
 [ OK ]	                          * should not match                   example.com
 [ OK ]	           xn--caf-dma*.com should not match              xn--caf-dmaa.com
 [ OK ]	           xn--caf*-dma.com should not match              xn--cafa-dma.com
 [ OK ]	   xn--caf-dma*.example.com should not match      xn--caf-dmaa.example.com
 [ OK ]	   xn--caf*-dma.example.com should not match      xn--cafa-dma.example.com
 [ OK ]	         w*.xn--caf-dma.com should     match           www.xn--caf-dma.com
 [ OK ]	          bar.*.example.net should not match             bar.a.example.net
 [ OK ]	              *.example.com should     match               foo.example.com
 [ OK ]	              *.example.com should not match           bar.foo.example.com
 [ OK ]	              *.example.com should not match                   example.com
 [ OK ]	           baz*.example.net should     match              baz1.example.net
 [ OK ]	           *baz.example.net should     match            foobaz.example.net
 [ OK ]	            b*z.example.net should     match              buzz.example.net
 [ OK ]	   xn--caf*-dpa.example.net should not match      xn--cafa-dpa.example.net
 [ OK ]	   xn--caf*-dpa.example.net should not match     xn--cafaa-dsa.example.net
 [ OK ]	   xn--*caf-epa.example.net should not match      xn--acaf-epa.example.net
 [ OK ]	    xn--c*f-dma.example.net should not match       xn--cxf-dma.example.net
 [ OK ]	            www.example.com should     match               www.example.com
 [ OK ]	              *.example.com should     match               www.example.com
 [ OK ]	             w*.example.com should     match               www.example.com
 [ OK ]	            ww*.example.com should     match               www.example.com
 [ OK ]	            Www.Example.com should     match               www.examPle.cOm
 [ OK ]	            www.e*ample.com should not match               www.example.com
 [ OK ]	             www.*ample.com should not match               www.example.com
 [ OK ]	                www.ex*.com should not match               www.example.com
 [ OK ]	                  www.*.com should not match               www.example.com
 [ OK ]	                      *.com should not match                   example.com
 [ OK ]	            www.example.com should not match                   example.com
 [ OK ]	                  *.abc.com should not match           www.example.abc.com
 [ OK ]	                        *.* should not match                   example.com
 [ OK ]	                          * should not match                       example
 [ OK ]	          a*.d*.example.com should not match           abc.def.example.com
 [ OK ]	                 *.*.com.au should not match            www.example.com.au
 [ OK ]	               www.*.com.au should not match            www.example.com.au
 [ OK ]	              *.example.com should not match                 ..example.com
 [ OK ]	              *.example.com should not match                  .example.com
 [ OK ]	             **.example.com should not match                ab.example.com
 [ OK ]	            *x*.example.com should not match               axb.example.com
 [ OK ]	               example.com. should     match                  example.com.
 [ OK ]	           www.example.com. should     match              www.example.com.
 [ OK ]	             *.example.com. should     match                a.example.com.
 [ OK ]	             *.example.com. should     match                b.example.com.
 [ OK ]	             *.example.com. should     match              www.example.com.
 [ OK ]	             *.example.com. should     match     xn--rger-koa.example.com.
 [ OK ]	         *.www.example.com. should     match         test.www.example.com.
 [ OK ]	          *www.example.com. should     match              www.example.com.
 [ OK ]	*.xn--rger-koa.example.com. should     match www.xn--rger-koa.example.com.
 [ OK ]	             *.xn--bar.com. should     match          xn--foo.xn--bar.com.
 [ OK ]	       *.good--example.com. should     match        www.good--example.com.
 [ OK ]	            t*.example.com. should     match             test.example.com.
 [ OK ]	            *t.example.com. should     match             test.example.com.
 [ OK ]	             *.example.com. should     match              www.example.com.
 [ OK ]	            f*.example.com. should     match             frog.example.com.
 [ OK ]	          baz*.example.net. should     match             baz1.example.net.
 [ OK ]	          *baz.example.net. should     match           foobaz.example.net.
 [ OK ]	           b*z.example.net. should     match             buzz.example.net.
 [ OK ]	        w*.xn--caf-dma.com. should     match          www.xn--caf-dma.com.
 [ OK ]	             *.example.com. should     match              foo.example.com.
 [ OK ]	          baz*.example.net. should     match             baz1.example.net.
 [ OK ]	          *baz.example.net. should     match           foobaz.example.net.
 [ OK ]	           b*z.example.net. should     match             buzz.example.net.
 [ OK ]	           www.example.com. should     match              www.example.com.
 [ OK ]	             *.example.com. should     match              www.example.com.
 [ OK ]	            w*.example.com. should     match              www.example.com.
 [ OK ]	           ww*.example.com. should     match              www.example.com.
 [ OK ]	           Www.Example.com. should     match              www.examPle.cOm.
 [ OK ]	               example.com. should     match                   example.com
 [ OK ]	           www.example.com. should     match               www.example.com
 [ OK ]	             *.example.com. should     match                 a.example.com
 [ OK ]	             *.example.com. should     match                 b.example.com
 [ OK ]	             *.example.com. should     match               www.example.com
 [ OK ]	             *.example.com. should     match      xn--rger-koa.example.com
 [ OK ]	         *.www.example.com. should     match          test.www.example.com
 [ OK ]	          *www.example.com. should     match               www.example.com
 [ OK ]	*.xn--rger-koa.example.com. should     match  www.xn--rger-koa.example.com
 [ OK ]	             *.xn--bar.com. should     match           xn--foo.xn--bar.com
 [ OK ]	       *.good--example.com. should     match         www.good--example.com
 [ OK ]	            t*.example.com. should     match              test.example.com
 [ OK ]	            *t.example.com. should     match              test.example.com
 [ OK ]	             *.example.com. should     match               www.example.com
 [ OK ]	            f*.example.com. should     match              frog.example.com
 [ OK ]	          baz*.example.net. should     match              baz1.example.net
 [ OK ]	          *baz.example.net. should     match            foobaz.example.net
 [ OK ]	           b*z.example.net. should     match              buzz.example.net
 [ OK ]	        w*.xn--caf-dma.com. should     match           www.xn--caf-dma.com
 [ OK ]	             *.example.com. should     match               foo.example.com
 [ OK ]	          baz*.example.net. should     match              baz1.example.net
 [ OK ]	          *baz.example.net. should     match            foobaz.example.net
 [ OK ]	           b*z.example.net. should     match              buzz.example.net
 [ OK ]	           www.example.com. should     match               www.example.com
 [ OK ]	             *.example.com. should     match               www.example.com
 [ OK ]	            w*.example.com. should     match               www.example.com
 [ OK ]	           ww*.example.com. should     match               www.example.com
 [ OK ]	           Www.Example.com. should     match               www.examPle.cOm
 [ OK ]	                example.com should     match                  example.com.
 [ OK ]	            www.example.com should     match              www.example.com.
 [ OK ]	              *.example.com should     match                a.example.com.
 [ OK ]	              *.example.com should     match                b.example.com.
 [ OK ]	              *.example.com should     match              www.example.com.
 [ OK ]	              *.example.com should     match     xn--rger-koa.example.com.
 [ OK ]	          *.www.example.com should     match         test.www.example.com.
 [ OK ]	           *www.example.com should     match              www.example.com.
 [ OK ]	 *.xn--rger-koa.example.com should     match www.xn--rger-koa.example.com.
 [ OK ]	              *.xn--bar.com should     match          xn--foo.xn--bar.com.
 [ OK ]	        *.good--example.com should     match        www.good--example.com.
 [ OK ]	             t*.example.com should     match             test.example.com.
 [ OK ]	             *t.example.com should     match             test.example.com.
 [ OK ]	              *.example.com should     match              www.example.com.
 [ OK ]	             f*.example.com should     match             frog.example.com.
 [ OK ]	           baz*.example.net should     match             baz1.example.net.
 [ OK ]	           *baz.example.net should     match           foobaz.example.net.
 [ OK ]	            b*z.example.net should     match             buzz.example.net.
 [ OK ]	         w*.xn--caf-dma.com should     match          www.xn--caf-dma.com.
 [ OK ]	              *.example.com should     match              foo.example.com.
 [ OK ]	           baz*.example.net should     match             baz1.example.net.
 [ OK ]	           *baz.example.net should     match           foobaz.example.net.
 [ OK ]	            b*z.example.net should     match             buzz.example.net.
 [ OK ]	            www.example.com should     match              www.example.com.
 [ OK ]	              *.example.com should     match              www.example.com.
 [ OK ]	             w*.example.com should     match              www.example.com.
 [ OK ]	            ww*.example.com should     match              www.example.com.
 [ OK ]	            Www.Example.com should     match              www.examPle.cOm.
 148 tests: 148 ok,   0 failed
```
