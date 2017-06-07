
window.getCurrentLayers = function(id) {
    return _.pluck($(id+' div.selected'), 'value');
};

// LAYER MANAGER CREATER`
window.CreateLayerManager = function(group, activeLayers) {
    var funcs = { },
        manager = { funcs: funcs },
        base_layer = [],
        base_prefix = [],
        fallback_prefix_fn = function(prefix, comparator) {

	   		// Extracting all cityPlan layers from SVG and collect year prefixes
			_.each(group, function(v, k){
				if(k.match('^(.*)-' + 'cityPlan' + '.*$')) {
					prefix.push(k.substring(0,4));
				}
			});
			
			prefix.sort().reverse();
    	
            var current_map_year = _.find(prefix, function(y){
                return parseInt(y, 10) <= parseInt(comparator, 10);
            });

            return (current_map_year || _.last(prefix)).toString();
        };

    manager.set_baseLayer = function(layers) {
        base_layer = layers;
        return manager;
    };

    manager.set_basePrefix = function(prefixes) {
        base_prefix = prefixes;
        return manager;
    };

    manager.set_fallbackPrefix = function(fn) {
        fallback_prefix_fn = fn;
    };

    // CREATE FUNCTION WITH CAPTURED-VARIABLE
    _.each(group, function(v, k) {
        var year = k.split('-')[0],
            obj = {};

        obj[k] = (function() {
            var captured_obj = v,
                ret = {
                    layer: k.split('-').slice(1).join('-') || 'no',
                    year: year,
                    hide: function() {
                        $(captured_obj).hide();
                    },
                    show: function() {
                        $(captured_obj).show();
                    },
                    data: function() {
                        return captured_obj;
                    }
                };
            ret.hide();
            return ret;
        })();


        _.extend(funcs, obj);
    });

    manager.show = function(year, layerArr, callback) {
        var prefix_to_show = base_prefix;
        var layer_to_show = _.union(base_layer, layerArr);

        // YEAR FOR ACCIDENT & SNAPPED YEAR FOR MAP-LAYOUT
        prefix_to_show = _.union(base_prefix, (year).toString(), fallback_prefix_fn([], year));

        var items_to_show = _.filter(funcs, function(item, k) {
            return _(prefix_to_show).contains(item.year) && _.contains(layer_to_show, item.layer);
        });


        // HIDE ACTIVE LAYER
        _.each(activeLayers, function(v) {
            v.hide();
        });

        // SET NEW ACTIVE LAYER
        activeLayers = items_to_show;

        _.each(items_to_show, function(item) { item.show(); });

        return manager;
    };

    manager.hide = function() {
        _.each(activeLayers, function(o) {
            o.hide();
        });
    };

    return manager;

}; // LAYER MNGR CREATER


// Utility

window.buildincidentHTML = function(idx, type) {
    var splitterMap = { 'graph-Event' : ' ', 'graph-Accident': '/' };
    var html = _.template("<div class='event-<%= type %>'>\
        <div class='incident-year'> <%= incident_year %> </div>\
        <ul id='incident-<%= type %>'>");
    var make_li = _.template(" <li>\
                                    <div class='time'><%= date %></div>\
                                    <div class='incident'> <%= incident %> </div>\
                                    <div class='detail'>\
                                    <% if (type=='graph-Accident') { %> \
                                        <span>สถานที่&nbsp;:&nbsp;</span> <%= location %><br />\
                                        <span>ผลกระทบ&nbsp;:&nbsp;</span> <%= effect %>\
                                    <% } %>\
                                    </div>\
                                </li>");
    var obj = window[type + 'List'];
    var html_bef;
    var html_body = "";
    var year = obj[idx].year;
    var html_image = "";

    if (!_.isUndefined(obj)) {
        _.each(obj[idx], function(v, k) {
            _.extend(v, {type: type, incident_year: year });
            html_bef = html(v);
            html_body += make_li(v);
            html_body += build_with_image(type, year, v['date']);
        });
    }

    html = html_bef + html_body + "</ul></div>";

    return  html;
};


window.build_with_image = function (type, year, date) {
    var html = "";
    var image_template = _.template("<div class='info-item'> \
                                        <img class='info-image' src='images/info_image/<%= filename %>'>\
                                        <p class='info-text'><%= info %></p> \
                                    </div>");

    var outcome = _.find(graph_bubble_info[year], function(oo) {
        return oo['date'] == date;
    })

    if (outcome && type === 'graph-Event') {
        html+= "<div class='info-wrapper'>";
        html+= image_template(outcome);
        html+= "</div>";
        return html;
    }
    else {
        return '';
    }
};



function bind_bubbleEvent(data, bubble) {
    var bindBubble = function(v, idx) {
        var t1, type, $opa, parent_cached;


        $(v).mouseenter(function(e) {
            var $this = $(this)
              , $parent = $this.parent();

            t1 = new Date();
            e.stopPropagation();

            if (_.isUndefined($parent.attr('id'))) {
                $this.css({'opacity': 1.0, 'cursor': 'pointer'});
                type = $this.parent().parent().parent().attr('id');
                $opa = $this.parent().parent();
                $opa.animate({opacity: 1.0}, 0);
            }
            else {
                $opa = undefined;
            }

            bubble.hide();


            // work around for move to top
            var obj = bubble.eq(idx);
            parent_cached = obj.parent();
            obj.parent().parent().append(obj);
            obj.fadeIn();

        })
        .click(function(e) {
            var obj = window[type + 'List'];
            var $parent = $(this).parent()
              , id = $parent.attr('id');

            if (_.isUndefined(id)) {
                var information = buildincidentHTML(idx, type);
                var height = (information.length)/2;

                $.colorbox({
                    html        : buildincidentHTML(idx, type),
                    width       : "600px",
                    opacity     : 0.82,
                    height      :  (height > 600 ? 600 : height) + 'px'
                });
            }

        })
        .mouseleave(function(e){
            var t2 = new Date()
              , diff = t2 - t1;
            var $this = $(this);

            var fadeOutCallback = function() {
                parent_cached.append(bubble.eq(idx));
            };

            if (diff>80) {
                setTimeout(function delayFadeOut() {
                    if ($opa) {
                        $opa.animate({opacity: 0.5});
                    }
                    bubble.eq(idx).fadeOut(fadeOutCallback);
                }, 200);
            }
            else {
                setTimeout(function muchDelayFadeOut() {
                    if ($opa) {
                        $opa.animate({opacity: 0.5});
                    }
                    bubble.eq(idx).fadeOut(fadeOutCallback);
                }, 300);
            }

        });
    };

    var circle = $('circle', data);
    _.each(circle, function(v, idx) {
        bindBubble(v, idx);
    });
}



function buildPMInfo(year) {
    var pm = pmList[year];
    var events_html = "<ul>";
    var events_template = _.template("<li><div class='time'><%= date %></div> <div class='data-year'><%= incident %></div> </li>");
    _.each(pm['events'], function(data) {
        events_html += events_template(data);
    });
    events_html += "</ul>";

    var template = _.template("<div class='primebox'> \
                                    <img src='asset/primeminister/<%= year %>-primeMinister.png'/> \
                                    <div class='fullname'> \
                                        <%= fullname %> \
                                    </div> \
                                    <div class='-bubbleldyear'> <%= hold_years %> </div> \
                                    <p class='events'>  <%= events_html %> </p> \
                                </div>");
    var data = { fullname: pm.fullname, hold_years: pm.hold_years,
        year: year, events_html: events_html
    };

    return  template(data);
}


var createControlButton = function(data, $target, callback, prefixId) {
    _.each(data, function(item, k){
        var layerId = 'layer'+k;
        var _prefixId = prefixId || '';

        if (_prefixId.length) {
            _prefixId = prefixId + '-';
        }

        elm = $('<div></div>').
            attr({
                id: _prefixId + layerId.toString(),
                value: item.key,
                'class': item['class'],
                'selected': true,
                'title': item['text']
            }).
            css({'float': 'left', width: '76px', height: '36px'});
        elm.addClass('selected');
        elm.mouseenter(function(e) {
            $(this).css({'opacity': 1.0, 'cursor': 'pointer'});
        });
        elm.click(function(e) {
            // e.preventDefault();
            var $this = $(this);
            var $attr = $this.attr('selected');
            if ($attr) {
                $this.removeClass('selected');
            }
            else {
                $this.addClass('selected');
            }
            $(this).attr('selected', !$attr);
            if (_.isFunction(callback)) {
                callback();
            }
        });

        // Initial tipsy.
        elm.tipsy({
            className: _prefixId + 'class ' + item['class'] + '-subclass',
            gravity: item['tipsyGravity']
        });

        $target.append(elm);
    });
};