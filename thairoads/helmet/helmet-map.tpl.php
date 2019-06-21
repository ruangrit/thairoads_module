<div id="maps" class="helmet-map <?php print $map_name;?>">
  <div class="map-header">
    <div class="img"><img src="/sites/all/themes/theme472/images/logo.png" /></div>
    <div class="date"><?php print date('d/m/Y');?></div>
  </div>
  <div class="map-title"><h2><?php print drupal_get_title();?></h2></div>
  <div class="maps-description">

    <h3>อัตราการสวมหมวกนริภัยรายจังหวัด<?php print $term_name; ?></h3>

    <p>ผลจากการสํารวจในภาคสนามด้วยวิธีการสังเกต (Observational Survey) ครอบคลุมพื้นที่ใน 77 จังหวัด ทั่วทั้งประเทศ </p>

    <h3>แหล่งข้อมูล</h3>

    <div>
      มูลนิธิไทยโรดส์และเครือข่ายเฝ้าระวังสถานการณ์ความปลอดภัยทางถนน (Road Safety Watch)
    </div>

  </div>
  <div style="display:none" id="warning_term_level" class='warning'>
  กำหนดขอบเขตไม่ครบ หรือ ยังไม่ได้กำหนดขอบเขตการแสดงสีของหมวดหมู่นี้
  </div>
  <div id="edit_term_level">
  <?php
  if (user_access('administer taxonomy')) {
    print l('ตั้งค่าขอบเขตการแสดงสีของหมวดหมู่นี้', 'admin/content/taxonomy/edit/term/'.$cate_id, array('query' => array('destination' => 'statistic/helmet/provincial/'.$cate_id)));
  }
  ?>
  </div>

  <div class="main-filter">
    <div class="filter-year form-group -helmet">

        <div class="filter-g1">
          <label for="filter1"> ข้อมูลสถิติในช่วงปี พ.ศ. </label>
          <select id="filter1" class="form-control filter-year-choice">
          </select>
        </div>
        
        <?php if (!is_null($filter_cat)):?>
          <div class="filter-cat">
            <label for="filter_cat"> ตำแหน่งที่นั่ง</label>
            <select id="filter_cat" class="form-control">
              <?php foreach ($filter_cat as $key => $value):?>
                <option value="<?php print $value;?>"><?php print $key;?></option>
              <?php endforeach;?>
            </select>
          </div>
        <?php endif;?>

    </div>
    <div class="filter-cat"></div>
  </div>
    
  <div class="map-heading">
    <div style="float: left; width: 85%;">อัตราการสวมหมวกนิรภัยของผู้ใช้รถจักรยานยนต์ <?php print $map_heading_type; ?> รายจังหวัด ปี <span class="year-info"></span></div>
    <div style="float: right;"><input type="button" value="<?php print t('print'); ?>" onclick="window.print();" class="print-btn -helmet"></div>
  </div>

  <div class="-stat-helmet">
  <div class="main-map">
  <?php $run_num_map = 1;?>
  <?php foreach($code_detail['id'] as $key => $value):?>

    <div class="map map-<?php print $key;?>">
      <!--
      <div class="print">
        <input type="button" value="<?php print t('print'); ?>" class="print-btn">
      </div>
      -->
      <div class="filter">
        <input type="hidden" class="code" value="<?php print $value;?>">
        <input type="hidden" class="cateId" value="<?php print $cate_id;?>">
      </div>
      <div class="map-description map-<?php print $key;?>"><?php print $code_detail['name'][$key];?> ปี <span class="year-info"></span></div>

      <div id="svgmap<?php print $key;?>">Loading...</div>

      <div class="top-ten <?php if($run_num_map != 1){ print 'hide';}?>">
        <div class="heading">
          10 จังหวัดที่มีอัตราการสวมหมวกนิรภัย <span class="code-detail"><?php print $code_detail['name'][$key];?></span> สูงสุด ปี <span class="year-info"></span>
        </div>
        <div class="data"></div>
      </div>
      <?php $run_num_map++;?>

      <div class="maps-info-province">
          <div id="svgmap<?php print $key;?>-info" class="info hide">
            <h3>ข้อมูลแต่ละจังหวัด</h3>
            <?php foreach($province_list as $k => $province):?>
              <?php if ($k == 0) { print '<div class="province-group group-first">'; } ?>
              <?php if ($k == 38) { print '<div class="province-group group-second">'; } ?>
              <div class="maps-info-item">
                <div class="maps-info-name"><?php print $province->name; ?></div>
                <div class="maps-info-value <?php print $province->tid;?>"></div>
              </div>
              <?php if ($k == 37 || $k == (count($province_list) - 1)) { print '</div>'; } ?>
            <?php endforeach;?>
          </div>
      </div>

      <!-- Color info-->
      <div class="map-info-color" id="info_color_<?php print $key;?>">
        <?php $run_num_info = 1; $map_unit = '%';?>
        <?php foreach($term_list as $key_term => $value_term):?>
          <?php if($run_num_info == 1):?>
            <div class="color-left">
            <?php if(($key == 2) && (arg(3) == 283)): ?>
              <div class="color-item">
                <div class="color">
                  <img src="/sites/all/modules/thairoads/helmet/img/level-0.png" />
                </div>
                <div class="level">
                  ไม่มีข้อมูลเขตชุมชนเมืองรอง
                </div>
              </div>
            <?php endif; ?>
          <?php endif;?>
          <?php if($run_num_info == 4):?>
            <div class="color-right">
          <?php endif;?>
          <div class="color-item">
            <div class="color">
              <img src="/sites/all/modules/thairoads/helmet/img/<?php print $value_term['img'];?>" />
            </div>
            <div class="level">
            <?php if($run_num_info != count($term_list) && $value_term['start'] != ''):?>
              <?php print thairoads_num_format($value_term['start'], 3).' - '.thairoads_num_format($value_term['end'], 3).' '.$map_unit; ?>
            <?php elseif($value_term['start'] != '' || $value_term['start'] == '-'):?>
              <?php print "> ".thairoads_num_format($value_term['start'], 3).' '.$map_unit;?>
            <?php else:?>
              ไม่มีข้อมูล
            <?php endif;?>
            </div>
          </div>
          <?php if($run_num_info == 3 || $run_num_info == 6):?>
            </div>
          <?php endif;?>
        <?php $run_num_info++;?>
        <?php endforeach;?>
        </div>
      </div>

    </div>

  <?php endforeach;?>

  </div>
</div>


<div class="map-url-wrapper" >
  <div class="map-url">URL: <?php print  "http://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]"; ?></div>
  <div class="map-sitename">มูลนิธิไทยโรดส์</div>
</div>
