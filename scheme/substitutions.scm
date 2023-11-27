(load-from-path "template.scm")

(define make-precursor-node
  (lambda* (nodes precursors #:optional rules)
           (let ((nodes-dependencies
                   (map 
                     (lambda (make-precursor-node)
                       (make-precursor-node nodes))
                     precursors))) 
             (make-schema-node 
               #:rule rules 
               #:dependent-nodes nodes-dependencies))))


(define make-precursor
  (lambda* (#:key rules precursors)
           (lambda* (#:optional nodes)
                    (if precursors
                        (make-precursor-node nodes precursors rules)
                        (make-schema-node #:rule rules #:dependent-nodes nodes)))))

(define double-precursor
  (make-precursor
    #:rules '((number-base-type . "double") 
              (struct-type-precision . "Double"))))

(define float-precursor
  (make-precursor
    #:rules '((number-base-type . "float") 
              (struct-type-precision . "Float"))))

(define number-precursor
  (make-precursor
    #:precursors (list double-precursor float-precursor)))

(define complex-number-precursor
  (make-precursor
    #:rules '((number-type . "${number-base-type} complex") 
              (struct-type-suffix . "Complex${struct-type-precision}")
              (function-tag . "_complex_${number-base-type}"))
    #:precursors (list number-precursor)))

(define real-number-precursor
  (make-precursor
    #:rules '((number-type . "${number-base-type}") 
              (struct-type-suffix . "Real${struct-type-precision}")
              (function-tag . "_real_${number-base-type}"))
    #:precursors (list number-precursor)))

(define real-number-schema
  (real-number-precursor))

(define numeric-precursor
  (make-precursor
    #:precursors (list complex-number-precursor  
                          real-number-precursor)))

(define vector-precursor
  (make-precursor
    #:rules '((vector-type . "Vector${struct-type-suffix}"))
    #:precursors (list numeric-precursor)))

(define vector-schema
  (vector-precursor))

(define filter-precursor
  (make-precursor
    #:rules '((filter-type . "DigitalFilter${struct-type-suffix}"))
    #:precursors (list vector-precursor)))

(define filter-schema
  (filter-precursor))
