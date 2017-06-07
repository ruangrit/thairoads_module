$(function() {
    var load_map = function(callback) {
        var svg = $('#svgload').svg({width: 300, height: 200});
        // LOAD SVG
        svg.load('map-test.svg', 'get', function(mapdata) {
            var activeLayers1 = [];
            var data = $('svg > g', $('#svgload'));
            var gr = _.groupBy(data, function(i) {
                return i.id.split('_')[0] || 'other';
            });

            // CREATE LAYER MANAGER
            var LayerManager = CreateLayerManager(gr, activeLayers1)
                .set_baseLayer(["road", "area", "accident", "cityPlan"])
                .set_basePrefix(["other", "all"]);

            callback(null, LayerManager);
        }); // LOAD SVG
    };

    var load_graph = function(callback) {
        var svggraph = $('#svggraph').svg();
        svggraph.load('graph.svg?'+Math.random(), 'get', function (svg) {
            var data = $('svg > g', $('#svggraph'));
            var activeLayers2 = [];

            var gr2 = _.groupBy(data, function(i) {
                return i.id.split('_')[0] || 'other';
            });

            // CREATE LAYER MANAGER
            var LayerManager2 = CreateLayerManager(gr2, activeLayers2); // LAYER MANAGER

            LayerManager2.set_fallbackPrefix(function() {
                return '';
            });
            callback(null, LayerManager2);
        });
    };


    var create_guide_manager = function(callback) {
        var act_guide = [];
        var __gr2__ = _.groupBy($('#graph-overlay g'), function(i) {
            return i.id.split('_')[0] || 'other';
        });

        var guideManager = CreateLayerManager(__gr2__, act_guide);
            guideManager.set_fallbackPrefix(function() {
                return '';
            });
        return guideManager;
    };

    var show_teaser = function(cb) {
        var text = '<div class="teaser">\
            <span>กำเนิดของนิคมอุตสาหกรรมมาบตาพุดนับเป็นก้าวสำคัญของการพัฒนาอุตสาหกรรมไทย</span>\
            แต่ตลอดระยะเวลาสามสิบปีที่ผ่านมา\
            การพัฒนานิคมแห่งนี้ถูกตั้งคำถามจากผู้มีส่วนได้เสีย โดยเฉพาะชุมชนโดยรอบ\
            มากขึ้นเรื่อยๆ จนกลายเป็นคดีประวัติศาสตร์ว่า\
            ภาคธุรกิจและรัฐแสดงความรับผิดชอบต่อปัญหาด้านความปลอดภัย มลพิษ\
            และปัญหาสังคมในพื้นที่อย่างไร ไทยพับลิก้าชวนทุกท่านร่วมกันประเมินอดีต\
            ปัจจุบัน เพื่อครุ่นคิดถึงอนาคตของมาบตาพุด ในอินเทอร์แอคทีฟ <strong>"30 ปี มาบตาพุด"</strong>    \
        </div>';


         $.colorbox({
            html        : text,
            width       :"800px",
            opacity     : 0.82,
            height      :  '340px'
        });

     cb(null, 'done');
};

    // LOAD SVG PARALLELY.
    async.parallel([load_map, load_graph, show_teaser], function (err, results) {
        // Construct operatons object
        var operations = {
            globalize: function(cb) {
                cb(null, 'done');
            },
            hook_startup: function(cb) {
                // var results = hook.startup();
                cb(null, 'first');
                // cb(null, results);
            },
            bind_bubbleEvents: function(cb) {
                bind_bubbleEvent(LayerManager2.funcs['graph-Factory'].data(), jQuery('[id$="factoryBubble"]'));
                bind_bubbleEvent(LayerManager2.funcs['graph-GPP'].data(), jQuery('[id$="gppBubble"]'));
                bind_bubbleEvent(LayerManager2.funcs['graph-Accident'].data(), jQuery('[id$="accidentBubble"]'));
                bind_bubbleEvent(LayerManager2.funcs['graph-Event'].data(), jQuery('[id$="event"]'));
                bind_bubbleEvent(LayerManager2.funcs['graph-people'].data(), jQuery('[id$="people-bubble"]'));
                bind_bubbleEvent(LayerManager2.funcs['graph-peopleHide'].data(), jQuery('[id$="peopleHide-bubble"]'));

                cb(null, 'done');
            },
            bind_graphEvents: function(cb) {
                var common_css = { 'z-index': 500, opacity: 0.0, width: '32', height: '32', background: 'red', position: 'absolute', top: '21px', 'float': 'left' };

                _.each(pm_face_position, function(i, k) {
                  $('#slider-range-max > div').append($('<div>').attr({id: k}).css(common_css).css(i));
                });

                // ON PRIMINISTER CLICK
                $('div[id$="primeMinister"]').
                    mouseover(function(e) {
                        $(this).css({ cursor: 'pointer'});

                        var pmId = $(this).attr('id').split('-')[0],
                            pmFullname = pmList[pmId] ? pmList[pmId]['fullname'] : '';
                        $(this).tipsy({
                            className: 'prime-minister-tipsy',
                            gravity: 'n',
                            fallback: 'รัฐบาล ' + pmFullname
                        })
                        .tipsy("show");
                    }).
                    mousedown(function(e){
                        e.stopPropagation();

                        var pmId = $(this).attr('id').split('-')[0],
                            information = buildPMInfo(pmId),
                            height = (information.length)/2;

                        height -= 55 * (Math.log(height) - 5);

                        $.colorbox({
                            html        : information,
                            width       :"600px",
                            opacity     : 0.82,
                            height      :  (height > 600 ? 600 : height) + 'px'
                        });
                    }); // mousedown

                cb(null, 'done');
            },
            create_graph_and_map_controller: function(cb) {
                var graph_control_labels = [{
                        "key": "Factory",
                        "class": "factory",
                        "text": "โรงงาน",
                        "tipsyGravity": "e"
                    },
                    {
                        "key": "Accident",
                        "class": "accident",
                        "text": "อุบัติเหตุ",
                        "tipsyGravity": "e"

                    },
                    {
                        "key": "Event",
                        "class": "event",
                        "text": "เหตุการณ์",
                        "tipsyGravity": "e"

                    },
                    {
                        "key": "GPP",
                        "class": "gpp",
                        "text": "จีพีพี (ผลิตภัณฑ์มวลรวมจังหวัด)",
                        "tipsyGravity": "e"
                    },
                    {
                        "key": "people",
                        "class": "people",
                        "text": "ประชากรท้องถิ่น",
                        "tipsyGravity": "e"
                    },
                    {
                        "key": "peopleHide",
                        "class": "peopleHide",
                        "text": "ประชากรแฝง",
                        "tipsyGravity": "e"
                    }
                ];

                var map_control_labels = [{
                            "key": "อาศัย",
                            "class": "home",
                            "text": "พื้นที่ที่อยู่อาศัย",
                            "tipsyGravity": "e"
                        }, {
                            "key": "อุตสาหกรรมคลังสินค้า",
                            "class": "factory",
                            "text": "พื้นที่อุตสาหกรรมหรือคลังสินค้า",
                            "tipsyGravity": "e"
                        }, {
                            "key": "พท-เกษตรกรรม",
                            "class": "agriculture",
                            "text": "พื้นที่เกษตรกรรม",
                            "tipsyGravity": "e"
                        }, {
                            "key": "อุตสาหกรรมไม่ก่อมลพิษ",
                            "class": "green-factory",
                            "text": "พื้นที่อุตสาหกรรมที่ไม่ก่อมลพิษ",
                            "tipsyGravity": "e"
                        }, {
                            "key": "พท-อนุรักษ์",
                            "class": "environment",
                            "text": "พื้นที่อนุรักษ์",
                            "tipsyGravity": "e"
                        }, {
                            "key": "ราชการ",
                            "class": "government",
                            "text": "พื้นที่ราชการ",
                            "tipsyGravity": "e"
                        }
                ];

                // TOGGLE
                createControlButton(map_control_labels, $('#map-control'), function(){
                    LayerManager.show(window.currentYear || 2531, getCurrentLayers('#map-control'));
                }, 'area-filter');

                createControlButton(graph_control_labels, $('#graph-control'), function(){
                    LayerManager2.show('graph', getCurrentLayers('#graph-control'));
                }, 'graph-filter');

                cb(null, 'done');
            },
            bind_click_accident_map: function(cb) {
                // Bind click on accident
                $('image[id*="accident"]').
                    mouseenter(function(e) {
                        $(this).css({cursor: 'pointer'});
                    }).
                    click(function(){
                        var $this = $(this);
                        var elmId = $this.attr('id').split('accident')[1];
                        var accId = parseInt(elmId, 10) - 1;
                        var template = _.template("<div class='accident-map'> \
                                                        <div class='acc_date'> <%= date %> </div> \
                                                        <div class='acc_desc'> <%= incident %> </div> \
                                                        <div class='acc_loc'><span>สถานที่: </span> <%= location %> </div>\
                                                        <div class='acc_eff'><span>ผลกระทบ: </span> <%= effect %> </div>\
                                                   </div>");
                        var acc_image = {
                            "2540": [{
                                "filename": "2540-01juneAccident.png",
                                "info": "วันที่ 23 มิย. 40 เกิดปัญหามลพิษทางอากาศ นักเรียนรร.มาบตาพุดพันพิทยาคารนับร้อยถูกหามส่งรพ."
                            }],
                            "2547": [{
                                "filename": "2547-02decAccident.png",
                                "info": "วันที่ 3 ธค. 47 เพลิงไหม้โกดังเก็บวัตถุดิบสำหรับผลิตพีวีซีหรือพลาสติกแผ่นแข็ง โรงงานซีพีเจริญโภคภัณฑ์ปิโตรเคมี จำกัด กิงอำเภอนิคมพัฒนา จ. ระยอง"
                            }],
                            "2555": [{
                                "filename": "2555-03decAccident.png",
                                "info": "วันที่ 5 ธค. 55 เหตุเพลิงไหม้โรงงานบ.บีเอสที อิลาสโตเมอร์ส จำกัด"
                            }]
                        };

                        var year = _.first($this.parent().attr('id').split('-'));
                        var siblings = $('image[id*="accident"]', $this.parent());
                        var ids = _.filter(_.collect(siblings, function(o) { return o.id }), function(oo) {
                            return oo.indexOf("Img") === -1;
                        });
                        var myid = $this.attr('id').replace("Img-", "");
                        var my_position = _.indexOf(ids, myid);
                        var data = accident_on_graph[year][my_position];
                        var html = template(data);

                        var image_template = _.template("<div class='info-item'> \
                                                        <img class='info-image' src='images/info_image/<%= filename %>'>\
                                                        <p class='info-text'><%= info %></p> \
                                                    </div>");
                        var factor = 0;

                        // add image if it exists
                        if (acc_image[year]) {
                            html+= "<div class='info-wrapper accident'>";
                                _.each(acc_image[year], function(oo) { html+= image_template(oo); });
                            html+= "</div>";
                            factor+= 310;
                        }

                        // bind popup-event
                        $.colorbox({
                            html        : html,
                            opacity     :0.8,
                            // transition  : 'fade',
                            width       : '560px',
                            height      : ((300+factor)).toString() +'px'
                            // transition: 'fade'
                        });


                });

                var text = '<div class="howtouse">\
                                <h3> วิธีการใช้: </h3> \
                                <ol>\
                                    <li>เลื่อนกรอบเวลาสีฟ้าในแกน X เพื่อดูผังเมืองและจุดเกิดอุบัติภัยในปีต่างๆ</li>\
                                    <li>คลิกสัญลักษณ์ต่างๆ ทางด้านขวามือ เพื่อ toggle ปิดหรือเปิดชุดข้อมูล</li>\
                                    <li>วางเมาส์และคลิกจุดต่างๆ ในกราฟและผังเมือง เพื่อดูรายละเอียดของเหตุการณ์</li>\
                                    <li>คลิกรูปนายกรัฐมนตรี เพื่ออ่านนโยบายสำคัญๆ ที่เกี่ยวข้องกับมาบตาพุด</li>\
                                </ol>\
                            </div>';


                $('.information').
                mouseover(function(e){
                    $(this).css({ cursor: 'pointer'});
                }).
                mousedown(function(e) {
                                        $.colorbox({
                                            html        : text,
                                            width       :"600px",
                                            opacity     : 0.82,
                                            height      :  '300px'
                                        });
                })

                cb(null, 'done');
            },
            bind_event_to_slider: function(cb) {
                var active_bubble = [];
                $( "#slider-range-max" ).slider({
                    value: window.currentYear,
                    min: 2520,
                    max: 2555,
                    step: 1,
                    animate: false,
                    slide: function( event, ui ) {
                        //For debug on screen
                        var val = $( "#amount" ).val( ui.value );
                        window.currentYear = ui.value;
                        var layer_to_show = getCurrentLayers('#map-control');
                        var year = ui.value;


                        var acc =  $('#'+year+'-accident', LayerManager2.funcs['graph-Accident'].data()).eq(0);
                        var evt =  $('#'+year+'-event_', LayerManager2.funcs['graph-Event'].data()).eq(0);

                        LayerManager.show(window.currentYear, layer_to_show);
                        guideManager.show(currentYear, ['graphOverlay']);

                        $.each(active_bubble, function(k, v) {
                          v.css({ 'opacity': '0.5'});
                        });

                        acc.css('opacity', 1.0);
                        evt.css('opacity', 1.0);
                        active_bubble.push(acc, evt);
                    }
                });

                //Update #amount (first time)
                $( "#amount" ).val( $( "#slider-range-max" ).slider( "value" ));

                cb(null, 'ok');
            },
            remove_indicator_and_show_slider: function(cb) {
                $('div.preloader').remove();
                $('div#slider-wrapper').show();
                cb(null, 'ok');
            },
            show_graph_and_map: function(cb) {
                var layer_to_show = getCurrentLayers('#map-control');

                LayerManager.show(currentYear, layer_to_show);
                LayerManager2.show('graph', ['Factory', 'Accident', 'Event', 'GPP', 'people', 'peopleHide']);

                $('#graph-overlay').show();

                guideManager.show(currentYear, ['graphOverlay']);

                cb(null, 'ok');
            }
        };

        // Do each operations serially
        if (!err) {
            window.currentYear = 2553;
            window.LayerManager  = results[0];
            window.LayerManager2 = results[1];
            window.guideManager = create_guide_manager();

            async.series(operations, function series_callback(err, results) {
                // console.log('results in series', results);
            });
        }
    });


});